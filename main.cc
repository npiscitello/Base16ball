#include <iostream>
#include "ball.h"

int main() {

  Ball ballhh(255, Ball::HEX, Ball::BIN);
  Ball ballhl(0,   Ball::HEX, Ball::BIN);
  Ball balloh(255, Ball::OCT, Ball::BIN);
  Ball ballol(0,   Ball::OCT, Ball::BIN);
  Ball ballbh(255, Ball::BIN, Ball::HEX);
  Ball ballbl(0,   Ball::BIN, Ball::HEX);

  std::cout << "Ballhh question: " << ballhh.question << std::endl;
  std::cout << "Ballhl question: " << ballhl.question << std::endl;
  std::cout << "Balloh question: " << balloh.question << std::endl;
  std::cout << "Ballol question: " << ballol.question << std::endl;
  std::cout << "Ballbh question: " << ballbh.question << std::endl;
  std::cout << "Ballbl question: " << ballbl.question << std::endl;
}
