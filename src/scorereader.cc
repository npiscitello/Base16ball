#include <iostream>
#include <fstream>
#include "proto/leaderboard.pb.h"

int main(int argc, char* argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  score::Leaderboard leaderboard;

  if( argc >= 2 ) {
    std::fstream input(argv[1], std::ios::in | std::ios::binary);
    if( !leaderboard.ParseFromIstream(&input) ) {
      std::cerr << "Bad filename!" << std::endl;
      return 1;
    }

    for( int i = 0; i < leaderboard.player_size(); i++) {
      const score::Player player = leaderboard.player(i);

      std::cout << "player name: " << player.name() << std::endl;
      std::cout << "player initials: " << player.initials() << std::endl;
      std::cout << "player score: " << player.score() << std::endl;
      std::cout << "player rank: " << player.rank() << std::endl;
    }
  } else {
    std::cerr << "Need to pass a filename as an arg!" << std::endl;
    return 1;
  }

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
