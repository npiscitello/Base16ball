// Ball class for base16ball
// Nick Piscitello, 11/2016

#ifndef BALL_H
#define BALL_H

#include <string>
#include <sstream>
// for uppercase, showbase, etc.
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <vector>

struct Ball {

  // the number formats that can be asked
  enum format_e {
    HEX = 1,
    OCT = 2,
    BIN = 3
  };

  // how many formats are contained in the format enum
  // there's probably a dynamic way to determine this?
  static const std::uint8_t NUM_FORMATS = 3;

  // a typedef for a vector that stores pairs of allowed conversions
  typedef std::vector<std::pair<format_e, format_e>> conversions_t;

  // the type of ball that can be thrown
  enum width_e {
    WIDTH8 = 1,
    WIDTH16 = 2
  };

  // storage for ball info
  // 8 or 16 bit 'ball'
  width_e type;
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
  Ball( width_e ty = WIDTH8, std::uint16_t number = 0, format_e from = HEX, format_e to = BIN ) :
    type(ty), num(number), from_fmt(from), to_fmt(to) {

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

#endif
