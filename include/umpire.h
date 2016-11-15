// Umpire class for base16ball
// Nick Piscitello, 11/2016

#ifndef UMPIRE_H
#define UMPIRE_H

#include "ball.h"
#include <random>
#include <chrono>
// std::transform
#include <algorithm>

// this class is exclusively ball management; it will not manage any graphics
struct Umpire {

  // constructor: set the difficulty of this umpire and which conversions are allowed
  // also seed the random number generator with the current time
  Umpire(Ball::width_e w, Ball::conversions_t(c)) : width(w), allowed_conversions(c) {
    rand_engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
  }

  // setters for width and allowed conversions
  void setWidth(Ball::width_e w) { width = w; }
  void setConversions(Ball::conversions_t c) { allowed_conversions = c; }

  // 'throw' a ball (generate ball and return it)
  // this method is starting out very verbose; it will be optimized
  Ball throwBall() {
    // randomly pick the conversion this ball is asking for
    Ball::conversions_t::value_type conversion;
    conversion = allowed_conversions.at(rand_engine() % allowed_conversions.size());
    // generate the ball with a random number based on the width
    Ball ball_to_throw;
    switch( width ) {
      case Ball::WIDTH8:
        ball_to_throw = Ball(width, rand_engine() % UINT8_MAX, conversion.first, conversion.second);
        break;
      case Ball::WIDTH16:
        ball_to_throw = Ball(width, rand_engine() % UINT16_MAX, conversion.first, conversion.second);
        break;
    }
    return ball_to_throw;
  }

  // check a user's answer against a ball
  bool checkBall(std::string answer, Ball ball) {
    std::transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
    std::transform(ball.answer.begin(), ball.answer.end(), ball.answer.begin(), ::toupper);
    return (answer.compare(ball.answer) == 0);
  }

  private:
    // the difficulty of the game
    Ball::width_e width;
    // the allowed conversions
    Ball::conversions_t allowed_conversions;
    // a random number generator, seeded with the current time
    std::default_random_engine rand_engine;
};

#endif
