#include "ncurses.h"

int main() {

  // start curses mode
  initscr();
  // print hello world to curses lib
  printw("Hello, world!");
  // print to real screen
  refresh();
  // wait for user input
  getch();
  // end curses mode
  endwin();

  return 0;
}
