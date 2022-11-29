#ifndef STAT_H_
#define STAT_H_

#include <string>
#include <vector>

#include "sql.h"

// Method to get the median from a sorted vector of values
float medianValue(std::vector<int> results);
// Method to get median, first and third quartile, and interquartile range
std::array<float, 4> percentileValues(std::vector<int> results);

int getNumTotalUsers(Database& db);

// if game_type == "", then return # all games
int getNumGames(Database& db, const std::string& game_type);

int getTotalPlayersForGame(Database& db, const std::string& game_type);

// return player_id
std::vector<std::string> getGreatestPlayerByWins(Database& db);

// return player_id
std::vector<std::string> getGreatestPlayerByEarnings(Database& db);

//Get median earnings for a given game.
int getMedianEarnings(Database& db, const std::string& game_type, const std::string& acct_id);

// TODO: may return JSON for compliated return type
// std::vector<std::string> getGameResultsGivenUserID(
//     Database& db, const std::string& userID);

#endif  // STAT_H_
