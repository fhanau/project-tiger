#include "gtest/gtest.h"
#include "../src/sqliteDB/sql.h"


TEST(Creating_Database, Return_Total_Number_Columns) {
    
    Database del_table = Database("delete.db");
    int players_stats_count;
    int game_list_count;
    int players_count;
    int achieve_count;
    int hosts_count;
    int games_count;

    sqlite3_stmt* one = del_table.makeStatement("SELECT * FROM player_stats");
    sqlite3_stmt* two = del_table.makeStatement("SELECT * FROM game_list");
    sqlite3_stmt* three = del_table.makeStatement("SELECT * FROM achievements");
    sqlite3_stmt* four = del_table.makeStatement("SELECT * FROM hosts");
    sqlite3_stmt* five = del_table.makeStatement("SELECT * FROM players");
    sqlite3_stmt* six = del_table.makeStatement("SELECT * FROM games");
    
    sqlite3_step(one);
    players_stats_count = sqlite3_column_count(one);

    sqlite3_step(two);
    game_list_count = sqlite3_column_count(two);

    sqlite3_step(three);
    achieve_count = sqlite3_column_count(three);

    sqlite3_step(four);
    hosts_count = sqlite3_column_count(four);

    sqlite3_step(five);
    players_count = sqlite3_column_count(five);

    sqlite3_step(six);
    games_count = sqlite3_column_count(six);

    
    
    EXPECT_EQ(players_stats_count, 9);
    EXPECT_EQ(game_list_count, 6);
    EXPECT_EQ(players_count, 2);
    EXPECT_EQ(achieve_count, 5);
    EXPECT_EQ(hosts_count, 2);
    EXPECT_EQ(games_count, 1);
    
}
