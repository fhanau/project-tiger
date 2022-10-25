#include "gtest/gtest.h"
#include "stat.h"

/**
Table: players
    player_id, host_id, name, lname
*/
// TODO: fix fixture class?
class StatTest:public::testing::Test {
 protected:
    void SetUp() override {
        db.deleteData("DELETE FROM players;");
        db.deleteData("DELETE FROM player_stats;");
        db.deleteData("DELETE FROM game_list;");
        db.deleteData("DELETE FROM achievements;");

        db.insertData("INSERT INTO player_stats(player_id, host_id, name, "
            "game_type, total_wins, total_losses, most_won, most_lost, total_money)"
            "VALUES(100, 500, 'Alex', 'RPS', 35, 53, 10000000, 50, 2222); ");

        db.insertData("INSERT INTO players(player_id, host_id, name, lname) "
            "VALUES(1, 8080, 'jack', 'black'), "
            "(2, 8080, 'lucy', 'alex'); ");
    }

    Database db = Database("dummy.db");
};


TEST_F(StatTest, testGetNumTotalUsers) {
    EXPECT_EQ(getNumTotalUsers(db), 2);
}

