#include <ncurses.h>
#include <menu.h>
#include <map>
#include "umpire.h"
#include "ball.h"

int main() {

  // determine which conversions are allowed
  Ball::conversions_t conversions;

  // screen size
  int row, col;

  // start curses mode with special key support
  initscr();
  keypad(stdscr, TRUE);
  // get the size of the screen
  getmaxyx(stdscr, row, col);

  // use a menu to get options
  {
    // define which items will be in the menu
    ITEM* items[5];
    items[0] = new_item("HEX -> BIN", "- Enable questions that give a hex number and ask for binary");
    items[1] = new_item("BIN -> HEX", "- Enable questions that give a binary number and ask for hex");
    items[2] = new_item("OCT -> BIN", "- Enable questions that give an octal number and ask for binary");
    items[3] = new_item("BIN -> OCT", "- Enable questions that give a binary number and ask for octal");
    // for some reason, the items list needs to be null terminated?
    items[4] = (ITEM*)NULL;

    // map conversions to menu items
    std::map<ITEM*, std::pair<Ball::format_e, Ball::format_e>> selections_map;
    selections_map.insert(std::make_pair(items[0], std::make_pair(Ball::HEX, Ball::BIN)));
    selections_map.insert(std::make_pair(items[1], std::make_pair(Ball::BIN, Ball::HEX)));
    selections_map.insert(std::make_pair(items[2], std::make_pair(Ball::OCT, Ball::BIN)));
    selections_map.insert(std::make_pair(items[3], std::make_pair(Ball::BIN, Ball::OCT)));

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
          menu_driver(menu, REQ_DOWN_ITEM);
          break;
        case 'k': // intentional fall-through
        case KEY_UP:
          menu_driver(menu, REQ_UP_ITEM);
          break;
        case ' ':
          // push the value at the key of the current item to the conversions vector
          // this isn't actually working! Maybe it's the multivalue thing?
          for( int i = 0; i < item_count(menu); i++ ) {
            if( item_value(items[i]) == TRUE ) {
              conversions.push_back(selections_map.find(items[i])->second);
            }
          }
          break;
      }
    }
    // remove menu from the screen and memory
    unpost_menu(menu);
    free_menu(menu);
    refresh();
  }

  mvprintw(10, 30, "conversions size: %d", conversions.size());
 
  // instantiate umpires
  Umpire ump8(Ball::WIDTH8, conversions);

  // ask new questions until we run out of screen space
  for( int i = 2; i < row - 2; i += 4 ) {
    // generate ball and convenience strings
    Ball ball = ump8.throwBall();
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
    if( ump8.checkBall(to_format + std::string(ans), ball) ) {
      mvprintw(i + 2, 5, "Correct!");
    } else {
      mvprintw(i + 2, 5, "Incorrect - the answer was %s", ball.answer.c_str());
    }
  }
  // end curses mode
  endwin();
}
