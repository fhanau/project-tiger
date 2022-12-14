#ifndef STAT_H_
#define STAT_H_

#include <string>
#include <vector>

#include "sql.h"

// Method to get the median from a sorted vector of values
float medianValue(std::vector<int> results);
// Method to get median, first and third quartile, and interquartile range
std::array<float, 4> percentileValues(std::vector<int> results);
// Method that returns above and below average outliers
std::array<std::vector<std::string>, 2> findOutlierUsers(Database& db,
  const std::string& sql_command);

int getNumTotalUsers(Database& db);

// if game_type == "", then return # all games
int getNumGames(Database& db, const std::string& game_type);

int getTotalPlayersForGame(Database& db, const std::string& game_type);

// TODO: may return JSON for compliated return type
// std::vector<std::string> getGameResultsGivenUserID(
//     Database& db, const std::string& userID);

#endif  // STAT_H_
