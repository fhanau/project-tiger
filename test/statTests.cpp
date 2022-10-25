#include "gtest/gtest.h"
#include "stat.h"

/**
Table: players
    player_id, username
*/
class StatTest:public::testing::Test {
 protected:
    void SetUp() override {
        db.deleteData("DELETE FROM players;");
        db.deleteData("DELETE FROM player_stats;");
        db.deleteData("DELETE FROM game_list;");
        db.deleteData("DELETE FROM achievements;");

        db.insertData("INSERT INTO players(player_id, username) \
            VALUES('1', 'jack'), \
            ('2', 'lucy'); "
        );
    }
    
    Database db = Database("dummy1.db");
};


TEST_F(StatTest, testGetNumTotalUsers) {
    EXPECT_EQ(getNumTotalUsers(db), 2);
}
