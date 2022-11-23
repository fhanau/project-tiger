#include <stdio.h>
#include <iostream>
#include "sql.h"

static int callback(void* NotUsed, int argc, char** argv, char** azColName);

int main() {
    Database dummy("dummy.db");
    std::string statsCommand = "INSERT INTO player_stats(player_id, host_id,"
      " name, game_type, total_wins, total_losses, most_won, most_lost, "
      "total_money) VALUES(100, 500, 'Alex', 'RPS', 35, 53, 10000000, 50, "
      "2222);";
    dummy.insertData(statsCommand);

    dummy.selectData("SELECT * FROM player_stats WHERE player_id = 100;");

    std::string updateCommand = "UPDATE player_stats SET player_id = 102 "
    "WHERE player_id = 100;";
    dummy.updateData(updateCommand);

    //dummy.deleteData("DELETE FROM player_stats WHERE player_id = 100;");

    sqlite3_stmt* x;
    std::string getStatsCommand = "SELECT * FROM player_stats WHERE player_id "
        "= 2100;";
    x = dummy.makeStatement(getStatsCommand);
    int col = sqlite3_column_count(x);

    std::cout << "START\n" << std::endl;
    std::cout << SQLITE_ROW << std::endl;

    if (doesExist(x) == 0) {
        std::cout << "CODE 2: FAILURE" << std::endl;
    } else {
        int test;
        std::cout << "SUCCESS" << std::endl;
        while ((test = sqlite3_step(x)) != SQLITE_DONE) {
            std::cout << "TEST = " << test << std::endl;
            for (int i = 0; i< col; i++) {
                std::cout << i << " Columns 9 = " <<
                sqlite3_column_count(x) <<" Num = " <<
                sqlite3_column_type(x, i) << std::endl;
                std::cout << test << std::endl;
            }
        }
    }
    /*
    if (test == SQLITE_DONE) {
        std::cout << "CODE 1" << std::endl;
    } else {
        while ((test = sqlite3_step(x)) != SQLITE_DONE) {
            std::cout << "TEST = " << test << std::endl;
            for(int i = 0; i< col; i++) {
                std::cout << i << " Columns 9 = " <<
                sqlite3_column_count(x) <<" Num = " <<
                sqlite3_column_type(x, i) << std::endl;
                std::cout << test << std::endl;
            }

        }
    }
    std::cout << "TEST AGAIN= " << test << std::endl;
   */
    return 0;
}
