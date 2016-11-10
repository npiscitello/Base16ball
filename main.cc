#include <ncurses.h>
#include <menu.h>
#include "umpire.h"
#include "ball.h"

int main() {

  // determine which conversions are allowed
  Ball::conversions_t conversions;
  conversions.push_back(std::make_pair(Ball::HEX, Ball::BIN));
  conversions.push_back(std::make_pair(Ball::BIN, Ball::HEX));
  conversions.push_back(std::make_pair(Ball::OCT, Ball::BIN));
  conversions.push_back(std::make_pair(Ball::BIN, Ball::OCT));

  // instantiate umpires
  Umpire ump8(Ball::WIDTH8, conversions);

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
    ITEM* items[6];
    items[0] = new_item("Item 1", "This is the first item");
    items[1] = new_item("Item 2", "This is the second item");
    items[2] = new_item("Item 3", "This is the third item");
    items[3] = new_item("Item 4", "This is the fourth item");
    items[4] = new_item("Item 5", "This is the fifth item");
    // for some reason, the items list needs to be null terminated?
    items[5] = (ITEM*)NULL;
    // create the menu
    MENU* menu = new_menu(items);
    // push the menu to the ncurses screen
    post_menu(menu);
    // refresh physical terminal
    refresh();
    int c = 0;
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
      }
    }
    // remove menu from the screen and memory
    unpost_menu(menu);
    free_menu(menu);
    refresh();
  }

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
