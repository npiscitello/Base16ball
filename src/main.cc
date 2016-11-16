#include <ncurses.h>
#include <menu.h>
#include <map>
// for animation
#include <chrono>
// for animation delay
#include <thread>
#include "umpire.h"
#include "ball.h"
#include "scoreboard.h"

// how many milliseconds to take scrolling the screen
std::uint32_t THROW_MS = 5000;
// side character offset
std::uint8_t RT_OFFSET = 20;
std::uint8_t LF_OFFSET = 5;

// use a menu to get options
Ball::conversions_t getConversions();
// use a menu to get difficulty
Ball::width_e getWidth();

int main() {

  // instantiate a scoreboard
  Scoreboard scoreboard("highscores.b16");
  scoreboard.setPlayerName("test");
  scoreboard.hit();
  scoreboard.saveScore("highscores.b16");

  // screen size
  int row, col;

  // start curses mode with special key support and a hidden cursor
  initscr();
  keypad(stdscr, TRUE);
  curs_set(0);
  // get the size of the screen
  getmaxyx(stdscr, row, col);

  // create windows
  WINDOW* ball_win = newwin(5, col-1, 0, 0);
  WINDOW* input_win = newwin(row-6, col-1, 5, 0);

  // don't wait for input on the input window
  nodelay(input_win, TRUE);

  // get the allowed conversions
  Ball::conversions_t conversions = getConversions();

  // only do stuff if the user selected something
  if( conversions.size() != 0 ) {

    // instantiate umpires
    Umpire ump(getWidth(), conversions);

    // generate ball and convenience strings
    Ball ball = ump.throwBall();
    std::string question  = "question: " + ball.question;
    std::string answer    = "answer: " + ball.answer;
    std::string to_format;
    switch( ball.to_fmt ) {
      case Ball::HEX:
        to_format = "0x";
        break;
      case Ball::OCT:
        to_format = "0";
        break;
      case Ball::BIN:
        to_format = "0b";
        break;
    }

    // ask for answer
    mvwprintw(input_win, 3, 5, "Your answer: %s", to_format.c_str());
    wrefresh(input_win);

    // animate question across the screen and check for answer
    char ans[20];
    uint8_t index = 0;
    std::chrono::milliseconds step_delay(THROW_MS / (col - (LF_OFFSET + RT_OFFSET)));
    for( int j = (col - RT_OFFSET); j > LF_OFFSET; j-- ) {
      mvwprintw(ball_win, 2, j, (question + " ").c_str());
      wrefresh(ball_win);
      // this is not the same 20 as the size of the array, that's a coincidence
      if( (ans[index] = mvwgetch(input_win, 3, 20 + index)) != ERR ) {
        if( ans[index] == '\n' ) {
          // replace the newline with a null byte
          ans[index] = '\0';
          break;
        } else {
          // append null byte after every added character
          ans[index + 1] = '\0';
        }
        index++;
      } else {
        // replace the error read with a null byte
        ans[index] = '\0';
      }
      // wait for the animation
      std::this_thread::sleep_for(step_delay);
    }

    // get/check answer and display result
    wmove(input_win, 4, 5);
    std::string processed_ans(to_format + std::string(ans));
    if( ump.checkBall(processed_ans, ball) ) {
      wprintw(input_win, "Correct!");
    } else {
      wprintw(input_win, "Incorrect - the answer was %s", ball.answer.c_str());
    }
    wrefresh(input_win);
  }
  // end curses mode
  getch();
  endwin();
}

// use a menu to get options
Ball::conversions_t getConversions() {
  // spin up storage for the conversions
  Ball::conversions_t conversions;

  // define which items will be in the menu
  ITEM* items[5];
  items[0] = new_item("HEX -> BIN", "- Enable questions that give a hex number and ask for binary");
  items[1] = new_item("BIN -> HEX", "- Enable questions that give a binary number and ask for hex");
  items[2] = new_item("OCT -> BIN", "- Enable questions that give an octal number and ask for binary");
  items[3] = new_item("BIN -> OCT", "- Enable questions that give a binary number and ask for octal");
  // for some reason, the items list needs to be null terminated?
  items[4] = (ITEM*)NULL;

  // map conversions to menu items
  std::map<ITEM*, Ball::conversions_t::value_type> selections_map;
  selections_map.insert(std::make_pair(items[0], std::make_pair(Ball::HEX, Ball::BIN)));
  selections_map.insert(std::make_pair(items[1], std::make_pair(Ball::BIN, Ball::HEX)));
  selections_map.insert(std::make_pair(items[2], std::make_pair(Ball::OCT, Ball::BIN)));
  selections_map.insert(std::make_pair(items[3], std::make_pair(Ball::BIN, Ball::OCT)));

  // create the menu
  MENU* menu = new_menu(items);
  // make the menu multi-valued
  menu_opts_off(menu, O_ONEVALUE);
  // push the menu to the ncurses screen
  post_menu(menu);
  // refresh physical terminal
  refresh();
  int c = 0;
  // the enter key is converted to a newline before being passed to ncurses
  while( (c = getch()) != '\n' ) {
    switch( c ) {
      case 'j': // intentional fall-through
      case KEY_DOWN:
        // move down
        menu_driver(menu, REQ_DOWN_ITEM);
        break;
      case 'k': // intentional fall-through
      case KEY_UP:
        // move up
        menu_driver(menu, REQ_UP_ITEM);
        break;
      case ' ':
        // select the current menu item
        menu_driver(menu, REQ_TOGGLE_ITEM);
        break;
    }
  }

  // push the value at the key of the selected items to the conversions vector
  // also, since we're iterating across items, free each item from memory after it's checked
  for( int i = 0; i < item_count(menu); i++ ) {
    if( item_value(items[i]) == TRUE ) {
      conversions.push_back(selections_map.find(items[i])->second);
    }
    free_item(items[i]);
  }

  // remove menu from the screen and memory
  unpost_menu(menu);
  free_menu(menu);
  refresh();

  return conversions;
}

Ball::width_e getWidth() {
  // spin up storage for the width
  Ball::width_e width;

  // define which items will be in the menu
  // <TODO> - implement a mode where both bitnesses can be used
  ITEM* items[3];
  items[0] = new_item("8-bit", "- use only 8 bit numbers");
  items[1] = new_item("16-bit", "- use only 16 bit numbers");
  // for some reason, the items list needs to be null terminated?
  items[2] = (ITEM*)NULL;

  // map conversions to menu items
  std::map<ITEM*, Ball::width_e> selections_map;
  selections_map.insert(std::make_pair(items[0], Ball::WIDTH8));
  selections_map.insert(std::make_pair(items[1], Ball::WIDTH16));

  // create the menu
  MENU* menu = new_menu(items);
  // push the menu to the ncurses screen
  post_menu(menu);
  // refresh physical terminal
  refresh();
  int c = 0;
  // the enter key is converted to a newline before being passed to ncurses
  while( (c = getch()) != '\n' ) {
    switch( c ) {
      case 'j': // intentional fall-through
      case KEY_DOWN:
        // move down
        menu_driver(menu, REQ_DOWN_ITEM);
        break;
      case 'k': // intentional fall-through
      case KEY_UP:
        // move up
        menu_driver(menu, REQ_UP_ITEM);
        break;
    }
  }

  // retrieve the selected difficulty
  width = selections_map.find(current_item(menu))->second;

  // free items from memory
  for( int i = 0; i < item_count(menu); i++ ) {
    free_item(items[i]);
  }

  // remove menu from the screen and memory
  unpost_menu(menu);
  free_menu(menu);
  refresh();

  return width;
}
