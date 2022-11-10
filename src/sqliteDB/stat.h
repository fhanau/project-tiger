#ifndef STAT_H_
#define STAT_H_

#include <string>
#include <vector>

#include "sql.h"


int getNumTotalUsers(const Database& db);

// if game_type == "", then return # all games
int getNumGames(Database& db, const std::string& game_type);

int getTotalPlayersForGame(Database& db, const std::string& game_type);

// return player_id
std::vector<std::string> getGreatestPlayerByWins(Database& db);

// return player_id
std::vector<std::string> getGreatestPlayerByEarnings(Database& db);

// TODO: may return JSON for compliated return type
// std::vector<std::string> getGameResultsGivenUserID(
//     Database& db, const std::string& userID);

#endif // STAT_H_
