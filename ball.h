// Ball class for base16ball
// Nick Piscitello, 2016

#include <string>
#include <sstream>
// for uppercase, showbase, etc.
#include <iomanip>
#include <bitset>
#include <cstdint>

struct Ball {

  // the number formats that can be asked
  enum format_e {
    HEX = 1,
    OCT = 2,
    BIN = 3
  };

  // the type of ball that can be thrown
  enum type_e {
    WIDTH8 = 1,
    WIDTH16 = 2
  };

  // storage for ball info
  // 8 or 16 bit 'ball'
  type_e type;
  // number of the 'ball'
  std::uint16_t num;
  // format of the question and answer
  format_e from_fmt;
  format_e to_fmt;
  // what the user is asked to convert
  std::string question;
  // the correct answer to the 'ball'
  std::string answer;

  // constructor - fill storage
  Ball( type_e ty = WIDTH8, std::uint16_t number = 0, format_e from = HEX, format_e to = BIN ) {
    type = ty;
    num = number;
    from_fmt = from; to_fmt = to;

    // width multiplier for question and answer
    std::uint8_t width_mult;
    switch( ty ) {
      case WIDTH8:
        width_mult = 1;
        break;
      case WIDTH16:
        width_mult = 2;
        break;
    }

    // generate question
    std::stringstream from_ss;
    from_ss << std::uppercase << std::setfill('0');
    switch( from ) {
      case HEX:
        from_ss << "0x" << std::hex << std::setw(2 * width_mult) << number;
        break;
      case OCT:
        from_ss << "0" << std::oct << std::setw(3 * width_mult) << number;
        break;
      case BIN:
        switch( ty ) {
          case WIDTH8:
            from_ss << "0b" << std::bitset<8>(number);
            break;
          case WIDTH16:
            from_ss << "0b" << std::bitset<16>(number);
            break;
        }
        break;
    }
    question = from_ss.str();

    // generate answer
    std::stringstream to_ss;
    to_ss << std::uppercase << std::setfill('0');
    switch( to ) {
      case HEX:
        to_ss << "0x" << std::hex << std::setw(2 * width_mult) << number;
        break;
      case OCT:
        to_ss << "0" << std::oct << std::setw(3 * width_mult) << number;
        break;
      case BIN:
        switch( ty ) {
          case WIDTH8:
            to_ss << "0b" << std::bitset<8>(number);
            break;
          case WIDTH16:
            to_ss << "0b" << std::bitset<16>(number);
            break;
        }

        break;
    }
    answer = to_ss.str();
  }
};
