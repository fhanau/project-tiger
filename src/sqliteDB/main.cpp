
#include <stdio.h>
#include <iostream>
#include "../libraries/sqlite/sqlite3.h"
#include "sql.h"

static int callback(void* NotUsed, int argc, char** argv, char** azColName);

int main() {
    Database dummy("dummy.db");

    dummy.insertData("INSERT INTO player_stats(player_id, host_id, name, \
game_type, total_wins, total_losses, most_won, most_lost, total_money) VALUES(100, 500, 'Alex', 'RPS', 35, 53, 10000000, 50, 2222); ");

    dummy.selectData("SELECT * FROM player_stats WHERE player_id = 100;");

    dummy.updateData("UPDATE player_stats SET player_id = 102 WHERE player_id = 100;");

    dummy.deleteData("DELETE FROM player_stats WHERE player_id = 100;");

    return 0;
}
