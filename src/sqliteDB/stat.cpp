#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <array>
#include "stat.h"


// helper function that executes commands
int runQueryWithIntReturn(Database& db, const std::string& sql_command) {
    sqlite3_stmt* stmt = db.makeStatement(sql_command);
    sqlite3_step(stmt);
    int ret = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return ret;
}

int runQueryWithIntReturn2(sqlite3_stmt* stmt) {
    int ret = sqlite3_column_int(stmt, 0);
    return ret;
}

float medianValue(std::vector<int> results) {
    int size = results.size();
    int middle = size / 2;
    if (size % 2 == 1 && size != 0) {
        return results.at(middle) * 1.0;
    } else if (size != 0) {
        int sum = results.at(middle) + results.at(middle - 1);
        float average = sum / 2.0;
        return average;
    } else {
        return 0.0;
    }
}

std::array<float, 4> percentileValues(std::vector<int> results) {
    std::array<float, 4> percentiles;
    int size = results.size();
    float median;
    int mid1;
    int mid2;


    // To create the sub-vectors for first and third quartile.
    if (size % 2 == 1 && size != 0) {
        mid1 = size / 2;
        mid2 = size / 2;
        median = results.at(mid1) * 1.0;
    } else if (size != 0) {
        mid1 = size / 2 - 1;
        mid2 = size / 2;
        median = (results.at(mid1) + results.at(mid2)) / 2.0;
    } else {
        percentiles = {0.0, 0.0, 0.0, 0.0};
        return percentiles;
    }

    std::vector<int> firstResults;
    std::vector<int> thirdResults;

    for (int i = 0; i < size; i++) {
        if (mid1 == mid2 && i == mid1) {
            continue;
        } else if (i <= mid1) {
            firstResults.push_back(results.at(i));
        } else {
            thirdResults.push_back(results.at(i));
        }
    }

    float firstQuartile = medianValue(firstResults);
    float thirdQuartile = medianValue(thirdResults);
    float interQuartileRange = thirdQuartile - firstQuartile;

    percentiles = {firstQuartile, median, thirdQuartile, interQuartileRange};

    return percentiles;
}

std::array<std::vector<std::string>, 2> findOutlierUsers(Database& db,
  const std::string& sql_command) {
    std::vector<int> results;
    std::vector<std::string> players;
    std::vector<std::string> abovePlayers = {""};
    std::vector<std::string> belowPlayers = {""};

    sqlite3_stmt* stmt = db.makeStatement(sql_command);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        int data = sqlite3_column_int(stmt, 0);
        std::string player_name = reinterpret_cast <const char*>(
          sqlite3_column_text(stmt, 1));

        results.push_back(data);
        players.push_back(player_name);
    }

    sqlite3_finalize(stmt);

    std::array<float, 4> boxStats = percentileValues(results);
    int belowStat = boxStats.at(0) - boxStats.at(3);
    int aboveStat = boxStats.at(2) + boxStats.at(3);

    for (int i = 0; i < results.size(); i++) {
        std::string player_name = players.at(i);
        int totalWins = results.at(i);
        if (totalWins < belowStat) {
            belowPlayers.push_back(player_name);
        } else if (totalWins > aboveStat) {
            abovePlayers.push_back(player_name);
        }
    }

    std::array<std::vector<std::string>, 2> exceptionalPlayers = \
      {belowPlayers, abovePlayers};

    return exceptionalPlayers;
}

int getNumTotalUsers(Database& db) {
    std::string command = "SELECT COUNT(*) FROM players;";
    return runQueryWithIntReturn(db, command);
}

int getNumGames(Database& db, const std::string& game_type) {
    std::string command;
    if (game_type == "") {
        command = "SELECT COUNT(DISTINCT game_id) FROM game_list;";
    } else {
        command = "SELECT COUNT(DISTINCT game_id) FROM game_list "
            "WHERE game_type = '" + game_type + "';";
    }
    return runQueryWithIntReturn(db, command);
}

int getTotalPlayersForGame(Database& db, const std::string& game_type) {
    std::string command = "SELECT COUNT(DISTINCT player_id) FROM game_list "
        "WHERE game_type = '" + game_type + "';";
    return runQueryWithIntReturn(db, command);
}
