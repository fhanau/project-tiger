#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "stat.h"

/**
Table: players
    player_id, username
*/
class StatTest:public::testing::Test {
 protected:
    void SetUp() override {
        // clear existing data
        // TODO: current buggy: Error in deleteData function.
        db.deleteData("DELETE FROM players;");
        db.deleteData("DELETE FROM player_stats;");
        db.deleteData("DELETE FROM game_list;");
        db.deleteData("DELETE FROM achievements;");
        // insert test data
        db.insertData(
            "INSERT INTO players(player_id, username)"
            "VALUES('player1', 'username1')," 
            "('player2', 'username2'),"
            "('player3', 'username4'),"
            "('player4', 'username4');"
        );
        db.insertData(
            "INSERT INTO player_stats(player_id, username, game_type, "
            "total_wins, total_losses, most_won, most_lost, total_money) "
            "VALUES('player1', 'username1', 'RPS', 1, 1, 1, 1, 0),"
            "('player1', 'username1', 'BlackJack', 1, 1, 1, 2, 0),"  
            "('player2', 'username2', 'RPS', 1, 1, 1, 0, 0)," 
            "('player3', 'username3', 'BlackJack', 1, 0, 1, -1, 0)," 
            "('player4', 'username4', 'BlackJack', 0, 1, 1, -1, 0);" 
        );
        db.insertData(
            "INSERT INTO game_list(game_id, game_type, username, "
            "player_id, result, earning) "
            "VALUES(100, 'RPS', 'username1', 'player1', '', '1'),"
            "(100, 'RPS', 'username2', 'player2', '', '-1'),"
            "(110, 'RPS', 'username1', 'player1', '', '-1'),"
            "(110, 'RPS', 'username2', 'player2', '', '-1'),"
            "(200, 'BlackJack', 'username1', 'player1', '', '-1'),"
            "(200, 'BlackJack', 'username3', 'player3', '', '-1'),"
            "(200, 'BlackJack', 'username4', 'player4', '', '2');"
        );
    }
    Database db = Database("dummy5.db");
};


TEST_F(StatTest, testGetNumTotalUsers) {
    EXPECT_EQ(getNumTotalUsers(db), 4);
}

TEST_F(StatTest, testGetNumGames) {
    EXPECT_EQ(getNumGames(db, ""), 3);
    EXPECT_EQ(getNumGames(db, "RPS"), 2);
    EXPECT_EQ(getNumGames(db, "BlackJack"), 1);
}

TEST_F(StatTest, testGetTotalPlayersForGame) {
    EXPECT_EQ(getTotalPlayersForGame(db, "RPS"), 2);
    EXPECT_EQ(getTotalPlayersForGame(db, "BlackJack"), 3);
    EXPECT_EQ(getTotalPlayersForGame(db, "doesNotExists"), 0);
}

// TEST_F(StatTest, testGetGreatestPlayerByWins) {
//     ASSERT_THAT(getGreatestPlayerByWins(db), testing::ElementsAre("player1"));
// }