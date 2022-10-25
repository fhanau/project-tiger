#include "stat.h"

#include <iostream>

int getNumTotalUsers(Database& db) {
    sqlite3_stmt* res = db.makeStatement(
        "SELECT COUNT(*) FROM players;");
    sqlite3_step(res);
    return sqlite3_column_int(res, 0);
}

int getNumGames(Database& db, const std::string& game_type) {
    sqlite3_stmt* res;
    if (game_type == "") {
        res = db.makeStatement(
            "SELECT COUNT(DISTINCT game_id) FROM game_list;");
    } else {
        res = db.makeStatement(
            "SELECT COUNT(DISTINCT game_id) FROM game_list " \
            "WHERE game_type = " + game_type);
    }
    sqlite3_step(res);
    return sqlite3_column_int(res, 0);
}
