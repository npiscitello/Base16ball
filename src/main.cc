#include <ncurses.h>
#include <menu.h>
#include <map>
// for animation
#include <chrono>
// for animation delay
#include <thread>
// for scoreboard testing
#include <iostream>
#include "umpire.h"
#include "ball.h"
#include "scoreboard.h"

// how many milliseconds to take scrolling the screen
const std::uint32_t THROW_MS = 5000;
// side character offset
const std::uint8_t RT_OFFSET = 20;
const std::uint8_t LF_OFFSET = 5;

// minimum terminal width
const std::uint8_t TERM_MIN_WIDTH = 66;

// input char array sizes
const std::uint8_t NAME_ARR_SIZE    = 64;
const std::uint8_t LG_ANS_ARR_SIZE  = 17;
const std::uint8_t SM_ANS_ARR_SIZE  = 9;

const std::string SCOREFILE = "highscores.b16";

// use a menu to get options
Ball::conversions_t getConversions(WINDOW* menu_win, int height, int width);
// use a menu to get difficulty
Ball::width_e getWidth(WINDOW* menu_win, int height, int width);

void printBanner(WINDOW* window, int y, int x) {
  // 58 characters wide, 7 lines tall
  mvwprintw(window, y + 0, x, "                        Welcome To");
  mvwprintw(window, y + 1, x, " _                   __   __  _           _ _      ____");
  mvwprintw(window, y + 2, x, "| |                 /_ | / / | |         | | |   .'    '.");
  mvwprintw(window, y + 3, x, "| |__   __ _ ___  ___| |/ /_ | |__   __ _| | |  /'-....-'\\");
  mvwprintw(window, y + 4, x, "| '_ \\ / _` / __|/ _ \\ | '_ \\| '_ \\ / _` | | |  |        |");
  mvwprintw(window, y + 5, x, "| |_) | (_| \\__ \\  __/ | (_) | |_) | (_| | | |  \\.-''''-./");
  mvwprintw(window, y + 6, x, "|_.__/ \\__,_|___/\\___|_|\\___/|_.__/ \\__,_|_|_|   '.____.'");
}

void printBall(WINDOW* window, int y, int x) {
  // 6 chars wide, 3 lines tall
 mvwprintw(window, y + 0, x, " .--. ");
 mvwprintw(window, y + 1, x, "(`-..)");
 mvwprintw(window, y + 2, x, " `--' ");
}

void printPlayer(WINDOW* window, int y, int x) {
  // 14 chars wide, 12 lines tall
  mvwprintw(window, y + 0, x,  "            _");
  mvwprintw(window, y + 1, x,  "           / )");
  mvwprintw(window, y + 2, x,  "          / /");
  mvwprintw(window, y + 3, x,  "   .-\"\"-.//'");
  mvwprintw(window, y + 4, x,  "  /_____C\\___");
  mvwprintw(window, y + 5, x,  "  /// 6 6~\\~~`");
  mvwprintw(window, y + 6, x,  "  (    7  )");
  mvwprintw(window, y + 7, x,  "   \\  '='/");
  mvwprintw(window, y + 8, x,  " _//'---'\\");
  mvwprintw(window, y + 9, x,  "( \\       `\\");
  mvwprintw(window, y + 10, x, "(\\/`-.__/  /");
  mvwprintw(window, y + 11, x, " \"`-._  _.'");
}

int main() {

   /*========================*\
  |     GAME INITIALIZATION    |
   \*========================*/

  // start curses mode
  initscr();
  // get the size of the screen
  int row, col;
  getmaxyx(stdscr, row, col);

  if( col < TERM_MIN_WIDTH ) {
    mvprintw(0, 0, "Terminal must be at least");
    mvprintw(1, 0, "%d characters wide.", TERM_MIN_WIDTH);
    mvprintw(2, 0, "Press any key to quit.");
    getch();
    endwin();
    return 1;
  }

  // intro sequence
  printBanner(stdscr, 3, (col / 2) - 29);

  // instantiate a scoreboard
  Scoreboard scoreboard(SCOREFILE);

  {
    char name[NAME_ARR_SIZE];
    mvprintw(11, (col / 2) - 29, "Player name: ");
    refresh();
    getstr(name);
    scoreboard.setPlayerName(std::string(name));
  }

  // set up the menu window
  WINDOW* menu_win = newwin(10, col, 14, 0);
  keypad(menu_win, TRUE);
  // globally make the cursor invisible
  curs_set(0);

  // get the allowed conversions
  Ball::conversions_t conversions;
  while( conversions.size() == 0 ) {
    conversions = getConversions(menu_win, 7, col - 2);
  }
  // get the desired difficulty and set answer array size
  uint8_t ANS_ARR_SIZE = 0;
  Ball::width_e width = getWidth(menu_win, 7, col - 2);
  switch( width ) {
    case Ball::WIDTH8:
      ANS_ARR_SIZE = SM_ANS_ARR_SIZE;
      break;
    case Ball::WIDTH16:
      ANS_ARR_SIZE = LG_ANS_ARR_SIZE;
      break;
  }


  // create windows
  WINDOW* ball_win = newwin(12, col-1, 0, 0);
  WINDOW* input_win = newwin(row-6, col-1, 12, 0);

  // don't wait for input on the input window
  nodelay(input_win, TRUE);

   /*========================*\
  |         GAMEPLAY           |
   \*========================*/

  // instantiate umpires
  Umpire ump(width, conversions);

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

  // display question and print answer
  mvwprintw(input_win, 2, 5, "Question: %s", ball.question.c_str());
  mvwprintw(input_win, 3, 5, "Your answer: %s", to_format.c_str());
  wrefresh(input_win);

  // animate question across the screen and check for answer
  char ans[ANS_ARR_SIZE]; ans[ANS_ARR_SIZE - 1] = '\0';
  uint8_t index = 0;
  std::chrono::milliseconds step_delay(THROW_MS / (col - (LF_OFFSET + RT_OFFSET)));
  for( int j = (col - RT_OFFSET); j > LF_OFFSET; j-- ) {
    mvwprintw(ball_win, 2, j, (question + " ").c_str());
    wrefresh(ball_win);
    if( (ans[index] = mvwgetch(input_win, 3, 20 + index)) != ERR ) {
      if( ans[index] == '\n' ) {
        // replace the newline with a null byte
        ans[index] = '\0';
        break;
      //} else if( ans[index] == '\b' ) {
      //  // backspace
      //  index--;
      } else if( index++ >= ANS_ARR_SIZE - 2 ) {
        index--;
      }
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
    scoreboard.hit();
  } else {
    wprintw(input_win, "Incorrect - the answer was %s", ball.answer.c_str());
    scoreboard.strike();
  }
  mvwprintw(input_win, 5, 5, "You put %s", processed_ans.c_str());
  wrefresh(input_win);

  // save the leaderboard
  scoreboard.saveScore(SCOREFILE);

  // end curses mode
  getch();
  endwin();
}

// use a menu to get options
Ball::conversions_t getConversions(WINDOW* menu_win, int height, int width) {
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
  // set it's window and subwindow
  set_menu_win(menu, menu_win);
  set_menu_sub(menu, derwin(menu_win, height, width, 2, 1));

  // print menu border and title
  box(menu_win, 0, 0);
  mvwprintw(menu_win, 0, (width/2) - 26, "Conversions: what kinds of questions you will be asked");
  mvwprintw(menu_win, 1, (width/2) - 32, "<K/J> or <UP/DOWN> to move, <SPACE> to select, <ENTER> to confirm");
  refresh();

  // make the menu multi-valued
  menu_opts_off(menu, O_ONEVALUE);
  // push the menu to the ncurses screen
  post_menu(menu);
  // refresh physical terminal
  wrefresh(menu_win);

  int c = 0;
  // the enter key is converted to a newline before being passed to ncurses
  while( (c = wgetch(menu_win)) != '\n' ) {
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
  wclear(menu_win);
  wrefresh(menu_win);

  return conversions;
}

Ball::width_e getWidth(WINDOW* menu_win, int height, int width) {
  // spin up storage for the width
  Ball::width_e bitwidth;

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
  // set it's window and subwindow
  set_menu_win(menu, menu_win);
  set_menu_sub(menu, derwin(menu_win, height, width, 2, 1));

  // print menu border and title
  box(menu_win, 0, 0);
  mvwprintw(menu_win, 0, (width/2) - 21, "Difficulty: bit-width of numbers presented");
  mvwprintw(menu_win, 1, (width/2) - 23, "<J/K> or <UP/DOWN> to move, <ENTER> to select");
  refresh();

  // push the menu to the ncurses screen
  post_menu(menu);
  // refresh physical terminal
  wrefresh(menu_win);
  int c = 0;
  // the enter key is converted to a newline before being passed to ncurses
  while( (c = wgetch(menu_win)) != '\n' ) {
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
  bitwidth = selections_map.find(current_item(menu))->second;

  // free items from memory
  for( int i = 0; i < item_count(menu); i++ ) {
    free_item(items[i]);
  }

  // remove menu from the screen and memory
  unpost_menu(menu);
  free_menu(menu);
  wclear(menu_win);
  wrefresh(menu_win);

  return bitwidth;
}
