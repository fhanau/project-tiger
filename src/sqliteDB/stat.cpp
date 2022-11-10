#include "stat.h"

#include <iostream>

// helper function that executes commands
int runQueryWithIntReturn(Database& db, const std::string& sql_command) {
    sqlite3_stmt* res;
    res = db.makeStatement(sql_command);
    sqlite3_step(res);
    return sqlite3_column_int(res, 0);
}

std::string runQueryWithSingleReturn(Database& db,
    const std::string& sql_command) {
    sqlite3_stmt* res;
    res = db.makeStatement(sql_command);
    sqlite3_step(res);
    // check https://en.cppreference.com/w/cpp/language/reinterpret_cast
    // const unsigned char* -> const char* -> string
    std::string ret{reinterpret_cast<const char*>(
        sqlite3_column_text(res, 0)
        )};
    return ret;
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
