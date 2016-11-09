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
  // throw and display ball
  Ball ball = ump8.throwBall();
  std::string question  = "question: " + ball.question;
  std::string answer    = "answer: " + ball.answer;
  mvprintw(row/3, (col - question.size())/3, question.c_str());
  mvprintw(2*(row/3), 2*((col - answer.size())/3), answer.c_str());
  // print to the real screen
  refresh();
  // wait for user input
  getch();
  // end curses mode
  endwin();
}
