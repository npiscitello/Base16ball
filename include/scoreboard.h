#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <fstream>
#include "proto/leaderboard.pb.h"

// This class is the clerical class, in charge of handling player score and status
class Scoreboard {

  public:

    // constructor, read an existing leaderboard from disk
    Scoreboard(std::string filename) {
      GOOGLE_PROTOBUF_VERIFY_VERSION;
      std::fstream infile(filename, std::ios::in | std::ios::binary);
      // check return value for errors!
      leaderboard.ParseFromIstream(&infile);
      player = leaderboard.add_player();
      player->set_score(0);
      infile.close();
    }

    // write the leaderboard to disk
    bool saveScore(std::string filename) {
      std::fstream outfile(filename, std::ios::out | std::ios::trunc | std::ios::binary);
      bool wrote = leaderboard.SerializeToOstream(&outfile);
      outfile.close();
      return wrote;
    }

    // player performance
    // Player answered the question wrong or took too long!
    void strike() { strikes++; }
    // Player answered correctly but not fast enough
    void ball() { player->set_score(player->score() + BALL_PTS); }
    // Player answered correctly fast enough
    void hit() { player->set_score(player->score() + HIT_PTS); }

    // returns true if the strike limit has been hit, resets strike count if so
    bool struckOut() {
      if( strikes >= MAX_STRIKES ) {
        strikes = 0;
        return true;
      } else {
        return false;
      }
    }

    // name and initial setters
    void setPlayerName(std::string name) { player->set_name(name); }
    void setPlayerInitials(std::string init) { player->set_initials(init); }

    // getters
    int getStrikes() { return strikes; }
    uint32_t getScore() { return player->score(); }

  private:

    // the saved leaderboard
    score::Leaderboard leaderboard;

    // the current player
    score::Player* player;

    // point value assigned to various events
    const int BALL_PTS = 1;
    const int HIT_PTS = 3;

    // strike details
    int strikes = 0;
    const int MAX_STRIKES = 3;
};

#endif
