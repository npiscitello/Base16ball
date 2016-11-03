// Ball class for base16ball
// Nick Piscitello, 2016

#include <string>
#include <sstream>

struct Ball {

  enum format_e {
    HEX = 1,
    OCT = 2,
    BIN = 3
  }

  std::uint8_t num;
  format_e from_fmt;
  format_e to_fmt;
  std::string question;
  std::string answer;

  Ball( std::uint8_t number, format_e from, format_e to ) {
    num = number;
    from_fmt = from; to_fmt = to;
    std::stringstream temp;
    switch( from ) {
      case HEX:
        temp << std::hex << number;
        break;
      case OCT:
        temp << std::oct << number;
        break;
      case BIN:
        // write binary
        break;
    }
    // copy above case for to as well
  }
}
