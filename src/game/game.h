#ifndef SRC_GAME_GAME_H_
#define SRC_GAME_GAME_H_

#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

#include "user.h"
#include "deck.h"

class Game {
 private:
    int numPlayers;
    std::string gameType;
    std::vector<User*> players;
    std::queue<User*> playerTurns;
    Deck* deck;

 public:
    Game(int numPlayers, std::string gameType);
    // void play();
    // void getResult();
    // void updateUserData();
    // void displayGameResult();
};

#endif // SRC_GAME_GAME_H_
