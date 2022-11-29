#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <array>
#include <algorithm>
#include "stat.h"


// helper function that executes commands
int runQueryWithIntReturn(Database& db, const std::string& sql_command) {
    sqlite3_stmt* stmt = db.makeStatement(sql_command);
    sqlite3_step(stmt);
    int ret = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return ret;
}

int runQueryWithIntReturn2(sqlite3_stmt* stmt ) {
    int ret = sqlite3_column_int(stmt, 0);
    return ret;
}

std::string runQueryWithSingleReturn(Database& db,
    const std::string& sql_command) {
    sqlite3_stmt* stmt = db.makeStatement(sql_command);
    sqlite3_step(stmt);

    const char* ret_str = reinterpret_cast <const char*>(
        sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    return std::string(ret_str);
}

// This method is to return a vector of ints
// to be used for computing calculations.
std::vector<int> pulledIntDataVector(Database& db,
  const std::string& sql_command) {
    std::vector<int> results;
    sqlite3_stmt* stmt = db.makeStatement(sql_command);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        int data = runQueryWithIntReturn2(stmt);
        results.push_back(data);
    }
    sqlite3_finalize(stmt);
    std::sort(results.begin(), results.end());
    /*
    std::cout << "RESULTS:";
    for (int i = 0; i < results.size(); i++)
        std::cout << ' ' << results.at(i);
    std::cout << '\n';
    */
    return results;
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

// TODO: buggy, segmentation fault
// return player_id
/*
std::vector<std::string> getGreatestPlayerByWins(Database& db) {
    std::vector<std::string> res;
    std::string command =
        "SELECT player_id, SUM(total_wins) AS tw FROM player_stats"
        "WHERE tw = (SELECT MAX(total_wins) FROM player_stats GROUP BY "
        "player_id)"
        "GROUP BY player_id";
    auto x = db.makeStatement(command);
    while (sqlite3_step(x) != SQLITE_DONE) {
        // const unsigned char* -> const char* -> string
        std::string player_id{
            reinterpret_cast<const char*>(sqlite3_column_text(x, 0))
        };
        res.push_back(player_id);
    }
    return res;
}
*/
