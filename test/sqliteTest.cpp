#include <sqlite3.h>
#include "gtest/gtest.h"
#include "../src/sqliteDB/sql.h"

TEST(Database_Create_and_Insert, Check_Insert_and_Create_methods) {
    Database del_table = Database("delete.db");
    int players_stats_count;
    int game_list_count;
    int players_count;
    int achieve_count;
    int hosts_count;
    int games_count;


    std::string command1 = "INSERT INTO player_stats(player_id, username,"
      " game_type, total_wins, total_losses, most_won, most_lost, "
      "total_money) VALUES('thePlayer', 'GuyMan', 'RPS', 35, 53, 100000, 50, "
      "2222);";

    std::string command2 = "INSERT INTO game_list(game_id, game_type,"
      " username, player_id, result, earning) VALUES(300, 'RPS', 'GuyMan',"
      " 'thePlayer', 'W', 100000);";

    std::string command3 = "INSERT INTO achievements(player_id, username,"
      " achievement_id, description, unlocked) VALUES('thePlayer', "
      "'GuyMan', 7, 'Win a Game', 1);";

    std::string command4 = "INSERT INTO hosts(username, password) "
      "VALUES('GuyMan', 'passwordSalted');";

    std::string command5 = "INSERT INTO players(player_id, username) "
      "VALUES('thePlayer', 'GuyMan');";

    std::string command6 = "INSERT INTO games(game_name) "
      "VALUES('RPS');";

    del_table.insertData(command1);
    del_table.insertData(command2);
    del_table.insertData(command3);
    del_table.insertData(command4);
    del_table.insertData(command5);
    del_table.insertData(command6);


    sqlite3_stmt* one = del_table.makeStatement("SELECT * FROM player_stats");
    sqlite3_stmt* two = del_table.makeStatement("SELECT * FROM game_list");
    sqlite3_stmt* three = del_table.makeStatement("SELECT * FROM achievements");
    sqlite3_stmt* four = del_table.makeStatement("SELECT * FROM hosts");
    sqlite3_stmt* five = del_table.makeStatement("SELECT * FROM players");
    sqlite3_stmt* six = del_table.makeStatement("SELECT * FROM games");
    sqlite3_step(one);
    players_stats_count = sqlite3_column_count(one);
    sqlite3_reset(one);

    sqlite3_step(two);
    game_list_count = sqlite3_column_count(two);
    sqlite3_reset(two);

    sqlite3_step(three);
    achieve_count = sqlite3_column_count(three);
    sqlite3_reset(three);

    sqlite3_step(four);
    hosts_count = sqlite3_column_count(four);
    sqlite3_reset(four);

    sqlite3_step(five);
    players_count = sqlite3_column_count(five);
    sqlite3_reset(five);

    sqlite3_step(six);
    games_count = sqlite3_column_count(six);
    sqlite3_reset(six);

    EXPECT_EQ(players_stats_count, 8);
    EXPECT_EQ(game_list_count, 6);
    EXPECT_EQ(players_count, 2);
    EXPECT_EQ(achieve_count, 5);
    EXPECT_EQ(hosts_count, 2);
    EXPECT_EQ(games_count, 1);
}

TEST(Database_Update, Check_Update_method) {
    Database del_table = Database("delete.db");

    std::string command1 = "UPDATE games SET game_name = 'RPS2'"
        "WHERE game_name = 'RPS';";
    del_table.updateData(command1);
    sqlite3_stmt* seven = del_table.makeStatement("SELECT * FROM games");

    while (sqlite3_step(seven) != SQLITE_DONE) {
        const char *value = (const char *)sqlite3_column_text(seven, 0);
        const char *answer = "RPS2";
        int check = 2;
        if (strcmp(value, answer) == 0) {
            check = 1;
        } else {
            check = 0;
        }
        EXPECT_EQ(check, 1);
        break;
    }

    sqlite3_reset(seven);
}

TEST(Database_Delete, Check_Delete_method) {
    Database del_table = Database("delete.db");
    int players_stats_count;
    int game_list_count;
    int players_count;
    int achieve_count;
    int hosts_count;
    int games_count;

    del_table.deleteData("DELETE FROM player_stats;");
    del_table.deleteData("DELETE FROM game_list;");
    del_table.deleteData("DELETE FROM achievements;");
    del_table.deleteData("DELETE FROM hosts;");
    del_table.deleteData("DELETE FROM players;");
    del_table.deleteData("DELETE FROM games;");


    std::string longString = "SELECT * FROM achievements";

    sqlite3_stmt* one_1 = del_table.makeStatement("SELECT * FROM player_stats");
    sqlite3_stmt* two_1 = del_table.makeStatement("SELECT * FROM game_list");
    sqlite3_stmt* three_1 = del_table.makeStatement(longString);
    sqlite3_stmt* four_1 = del_table.makeStatement("SELECT * FROM hosts");
    sqlite3_stmt* five_1 = del_table.makeStatement("SELECT * FROM players");
    sqlite3_stmt* six_1 = del_table.makeStatement("SELECT * FROM games");
    sqlite3_step(one_1);
    players_stats_count = del_table.doesExist(one_1);
    sqlite3_step(two_1);
    game_list_count = del_table.doesExist(two_1);
    sqlite3_step(three_1);
    achieve_count = del_table.doesExist(three_1);
    sqlite3_step(four_1);
    hosts_count = del_table.doesExist(four_1);
    sqlite3_step(five_1);
    players_count = del_table.doesExist(five_1);
    sqlite3_step(six_1);
    games_count = del_table.doesExist(six_1);
    EXPECT_EQ(players_stats_count, 0);
    EXPECT_EQ(game_list_count, 0);
    EXPECT_EQ(players_count, 0);
    EXPECT_EQ(achieve_count, 0);
    EXPECT_EQ(hosts_count, 0);
    EXPECT_EQ(games_count, 0);
}

TEST(Database_Drop, Check_Drop_method) {
    Database del_table = Database("delete.db");

    del_table.dropTable2("DROP TABLE IF EXISTS player_stats;");

    std::string string1 = "SELECT COUNT(*) as theCount FROM sqlite_master ";
    std::string string2 = "WHERE type = 'table' AND name='player_stats';";
    std::string longString = string1 + string2;

    sqlite3_stmt* testt = del_table.makeStatement(longString);

    sqlite3_step(testt);

    int totalTables = sqlite3_column_int(testt, 0);

    EXPECT_EQ(totalTables, 0);
}
