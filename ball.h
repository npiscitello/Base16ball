// Ball class for base16ball
// Nick Piscitello, 2016

#include <string>
#include <sstream>
// for uppercase, showbase, etc.
#include <iomanip>
#include <bitset>
#include <cstdint>

struct Ball {

  enum format_e {
    HEX = 1,
    OCT = 2,
    BIN = 3
  };

  std::uint8_t num = 5;
  format_e from_fmt;
  format_e to_fmt;
  std::string question;
  std::string answer;

  Ball( std::uint8_t number, format_e from, format_e to ) {
    num = number;
    from_fmt = from; to_fmt = to;
    std::stringstream temp;
    temp << std::uppercase << std::setfill('0');
    switch( from ) {
      case HEX:
        temp << std::hex << "0x" << std::setw(2) << (unsigned)number;
        break;
      case OCT:
        temp << std::oct << "0" << std::setw(3) << (unsigned)number;
        break;
      case BIN:
        temp << "0b" << std::bitset<8>((unsigned)number);
        break;
    }
    question = temp.str();
    // copy above case for to as well
  }
};
