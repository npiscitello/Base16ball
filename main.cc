#include <ncurses.h>
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

  // start curses mode
  initscr();
  // get the size of the screen
  getmaxyx(stdscr, row, col);

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
