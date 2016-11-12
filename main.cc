#include <ncurses.h>
#include <menu.h>
#include <map>
#include "umpire.h"
#include "ball.h"

// use a menu to get options
Ball::conversions_t getConversions();
// use a menu to get difficulty
Ball::width_e getWidth();

int main() {

  // screen size
  int row, col;

  // start curses mode with special key support
  initscr();
  keypad(stdscr, TRUE);
  // get the size of the screen
  getmaxyx(stdscr, row, col);

  // get the allowed conversions
  Ball::conversions_t conversions = getConversions();

  // only do stuff if the user selected something
  if( conversions.size() != 0 ) {

    // instantiate umpires
    Umpire ump(getWidth(), conversions);

    // ask new questions until we run out of screen space
    for( int i = 2; i < row - 2; i += 4 ) {
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

      // print to ncurses
      mvprintw(i, 5, question.c_str());
      mvprintw(i + 1, 5, "Your answer: %s", to_format.c_str());
      // print to the real screen
      refresh();

      // get/check answer and display result
      char ans[20];
      getstr(ans);
      if( ump.checkBall(to_format + std::string(ans), ball) ) {
        mvprintw(i + 2, 5, "Correct!");
      } else {
        mvprintw(i + 2, 5, "Incorrect - the answer was %s", ball.answer.c_str());
      }
    }
  }
  // end curses mode
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
