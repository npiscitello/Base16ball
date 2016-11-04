#include <iostream>
#include "ball.h"

int main() {

  std::cout << std::endl << "\x1B[32mBall Class Testapp\x1b[0m" << std::endl;

  Ball hbs(Ball::WIDTH8,  15,  Ball::HEX, Ball::BIN);
  Ball obs(Ball::WIDTH8,  15,  Ball::OCT, Ball::BIN);
  Ball bhs(Ball::WIDTH8,  15,  Ball::BIN, Ball::HEX);
  Ball bos(Ball::WIDTH8,  15,  Ball::BIN, Ball::OCT);
  Ball hbl(Ball::WIDTH16, 255, Ball::HEX, Ball::BIN);
  Ball obl(Ball::WIDTH16, 255, Ball::OCT, Ball::BIN);
  Ball bhl(Ball::WIDTH16, 255, Ball::BIN, Ball::HEX);
  Ball bol(Ball::WIDTH16, 255, Ball::BIN, Ball::OCT);

  std::cout << "dec: " << hbs.num << ", hex: " << hbs.question << 
    ", bin: " << hbs.answer << std::endl;
  std::cout << "dec: " << obs.num << ", oct: " << obs.question << 
    ", bin: " << obs.answer << std::endl;
  std::cout << "dec: " << bhs.num << ", bin: " << bhs.question << 
    ", hex: " << bhs.answer << std::endl;
  std::cout << "dec: " << bos.num << ", bin: " << bos.question << 
    ", oct: " << bos.answer << std::endl;
  std::cout << "dec: " << hbl.num << ", hex: " << hbl.question << 
    ", bin: " << hbl.answer << std::endl;
  std::cout << "dec: " << obl.num << ", oct: " << obl.question << 
    ", bin: " << obl.answer << std::endl;
  std::cout << "dec: " << bhl.num << ", bin: " << bhl.question << 
    ", hex: " << bhl.answer << std::endl;
  std::cout << "dec: " << bol.num << ", bin: " << bol.question << 
    ", oct: " << bol.answer << std::endl;

  std::cout << std::endl;
}
