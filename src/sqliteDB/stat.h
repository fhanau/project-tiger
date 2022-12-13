#ifndef STAT_H_
#define STAT_H_

#include <string>
#include <vector>

#include "sql.h"

// Method to get the median from a sorted vector of values
float medianValue(std::vector<int> results);
// Method to get median, first and third quartile, and interquartile range
std::array<float, 4> percentileValues(std::vector<int> results);
// Method to pull a list of ints from a sql command
std::vector<int> pulledIntDataVector(Database& db,
  const std::string& sql_command);
// Method that returns above and below average outliers
std::array<std::vector<std::string>, 2> findOutlierUsers(Database& db,
  const std::string& sql_command);

// TODO: may return JSON for compliated return type
// std::vector<std::string> getGameResultsGivenUserID(
//     Database& db, const std::string& userID);

#endif  // STAT_H_
