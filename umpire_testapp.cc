#include <iostream>
#include "umpire.h"
#include "ball.h"

int main() {

  // determine which conversions are allowed
  Ball::conversions_t conversions;
  conversions.push_back(std::make_pair(Ball::HEX, Ball::BIN));
  conversions.push_back(std::make_pair(Ball::BIN, Ball::HEX));
  conversions.push_back(std::make_pair(Ball::OCT, Ball::BIN));
  conversions.push_back(std::make_pair(Ball::BIN, Ball::OCT));

  // instantiate empires
  Umpire ump8(Ball::WIDTH8, conversions);
  Umpire ump16(Ball::WIDTH16, conversions);

  // throw and output balls
  Ball test8 = ump8.throwBall();
  Ball test16 = ump16.throwBall();
  std::cout << "8 bit question: " << test8.question << ", 8 bit answer: " << test8.answer << std::endl;
  std::cout << "16 bit question: " << test16.question << ", 16 bit answer: " << test16.answer << std::endl;

  return 0;
}
