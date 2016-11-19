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
const std::uint8_t RT_OFFSET = 12;
const std::uint8_t LF_OFFSET = 1;

// minimum terminal dimensions
const std::uint8_t TERM_MIN_WIDTH = 66;
const std::uint8_t TERM_MIN_HEIGHT = 24;

// input char array sizes
const std::uint8_t NAME_ARR_SIZE = 64;

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
  // 12 chars wide, 3 lines tall
  mvwprintw(window, y + 0, x, " .--. --- ");
  mvwprintw(window, y + 1, x, "(`-..) ---- ");
  mvwprintw(window, y + 2, x, " `--' --- ");
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

  if( row < TERM_MIN_HEIGHT || col < TERM_MIN_WIDTH ) {
    mvprintw(0, 0, "Terminal must be at least");
    mvprintw(1, 0, "%d characters wide and", TERM_MIN_WIDTH);
    mvprintw(2, 0, "%d characters tall.", TERM_MIN_HEIGHT);
    mvprintw(3, 0, "Press any key to quit.");
    getch();
    endwin();
    return 1;
  }

  // intro sequence
  printBanner(stdscr, 3, (col / 2) - 29);

  // instantiate scoreboard and get player name
  Scoreboard scoreboard(SCOREFILE);
  {
    char name[NAME_ARR_SIZE]; name[0] = '\0';
    while( name[0] == '\0' ) {
      mvprintw(11, (col / 2) - 29, "Player name: ");
      refresh();
      getstr(name);
    }
    scoreboard.setPlayerName(std::string(name));
  }

  // set up the menu window
  WINDOW* menu_win = newwin(10, col, 14, 0);
  keypad(menu_win, TRUE);
  // globally make the cursor invisible
  curs_set(0);
  // disable echoing
  noecho();

  // get the allowed conversions
  Ball::conversions_t conversions;
  while( conversions.size() == 0 ) {
    conversions = getConversions(menu_win, 7, col - 2);
  }
  // get the desired difficulty and set answer array size
  Ball::width_e width = getWidth(menu_win, 7, col - 2);

  // clear stdscr and create game windows - #lines, #columns, y0, x0
  erase(); refresh();
  WINDOW* player_win = newwin(12, 15, 0, 0);
  WINDOW* ball_win = newwin(12, col - 15, 0, 15);
  WINDOW* input_win = newwin(0, 0, 12, 0);

  // get size of ball window
  int brow, bcol;
  getmaxyx(ball_win, brow, bcol);

  // don't block for input on the input window
  nodelay(input_win, TRUE);

   /*========================*\
  |         GAMEPLAY           |
   \*========================*/

  // instantiate umpires
  Umpire ump(width, conversions);

  // display player
  printPlayer(player_win, 0, 1);
  wrefresh(player_win);
    
  mvwprintw(input_win, 2, 5, "Press any key to throw the first pitch...");
  wrefresh(input_win);
  getch();
  werase(input_win);

  while( true ) {
    // generate ball and convenience strings
    Ball ball = ump.throwBall();
    std::string question  = "question: " + ball.question;
    std::string answer    = "answer: " + ball.answer;
    std::string to_format;
    int answer_offset;
    switch( ball.to_fmt ) {
      case Ball::HEX:
        to_format = "0x";
        answer_offset = 20;
        break;
      case Ball::OCT:
        to_format = "0";
        answer_offset = 19;
        break;
      case Ball::BIN:
        to_format = "0b";
        answer_offset = 20;
        break;
    }

    // display question and print answer
    mvwprintw(input_win, 2, 5, "Question: %s", ball.question.c_str());
    mvwprintw(input_win, 3, 5, "Your answer: %s", to_format.c_str());
    wrefresh(input_win);

    // allocate answer storage; need 1 extra space for the null terminator
    char ans[ball.ans_str_len + 1]; ans[0] = '\0';
    char test; bool entered = false;
    uint8_t index = 0;
    std::chrono::milliseconds step_delay(THROW_MS / (bcol - (LF_OFFSET + RT_OFFSET)));
    // animate ball across the screen and check for answer
    // a better way to do this would be to run this loop at full speed and update the ball's
    // position based on checking if enough time has elapsed - as it stands, the game feels laggy in
    // small terminal windows because the animation delay is fairly long
    for( int i = (bcol - RT_OFFSET); i > LF_OFFSET; i-- ) {
      // get and test input character
      if( (test = wgetch(input_win)) != ERR ) {
        if( test == '\n' ) {
          // break loop if user presses enter
          entered = true;
          break;
        } else if( test == (char)127 || test == (char)8 ) {
          // decrement index if the cursor isn't at the very beginning or end
          // There are 2 states of the array when index = ball.ans_str_len: the value of
          // ans[ball.ans_str_len] = '\0' or the value of ans[ball.ans_str_len] is a
          // meaningful character. If it's a meaningful character, we don't want to decrement
          // the index because that will push the cursor 2 characters back since it's already
          // sitting at the last character.
          if( index >= 1 && (index < ball.ans_str_len - 1 || ans[index] == '\0') ) {
            index--;
          }
          mvwaddch(input_win, 3, answer_offset + index, ' ');
          ans[index] = '\0';
        } else {
          // write to screen and answer array
          mvwaddch(input_win, 3, answer_offset + index, test);
          ans[index] = test;
          ans[index + 1] = '\0';
          // increment index if it won't overflow the answer
          if( index < ball.ans_str_len - 1 ) {
            index++;
          }
        }
      }
      // animate!
      printBall(ball_win, 7, i);
      wrefresh(ball_win);
      std::this_thread::sleep_for(step_delay);
    }

    // get/check answer and display result
    // a correct answer entered with the enter key counts as a hit
    // a correct answer on timeout counts as a ball
    // any incorrect answer counts as a strike
    wmove(input_win, 4, 5);
    std::string processed_ans(to_format + std::string(ans));
    if( ump.checkBall(processed_ans, ball) ) {
      if( entered ) {
        wprintw(input_win, "Correct! You hit the ball!");
        scoreboard.hit();
      } else {
        wprintw(input_win, "Correct! The pitcher threw a ball.");
        scoreboard.ball();
      }
    } else {
      wprintw(input_win, "Incorrect - that's a strike! the answer was %s", ball.answer.c_str());
      scoreboard.strike();
    }

    // check game progress
    if( !scoreboard.struckOut() ) {
      mvwprintw(input_win, 5, 5, "Press any key to throw the next pitch...");
      wrefresh(input_win);
      getch();
      werase(input_win);
      werase(ball_win);
    } else {
      break;
    }
  }

  // struck out!
  mvwprintw(input_win, 6, 5, "Oh no! You've struck out!");
  mvwprintw(input_win, 7, 5, "Your score is being saved in '%s'.", SCOREFILE.c_str());
  mvwprintw(input_win, 8, 5, "Use 'scorereader' to see the leaderboard.");
  mvwprintw(input_win, 9, 5, "Give it another shot, see if you can beat your high score!");
  mvwprintw(input_win, 11, 5, "Press any key to exit...");
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
