#include <iostream>
#include <sqlite3.h>
#include "stat.h"

// helper function that executes commands
int runQueryWithIntReturn(Database& db, const std::string& sql_command) {
    sqlite3_stmt* stmt = db.makeStatement(sql_command);
    sqlite3_step(stmt);
    int ret = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
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
