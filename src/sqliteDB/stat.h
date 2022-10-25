#ifndef STAT_H_
#define STAT_H_

#include <string>

#include "sql.h"


int getNumTotalUsers(Database& db);

// if game_type == "", then return # all games
int getNumGames(Database& db, const std::string& game_type);

// TODO: may return JSON for compliated return type
// std::vector<std::string> getGameResultsGivenUserID(
//     Database& db, const std::string& userID);

#endif // STAT_H_
