#include "gtest/gtest.h"
#include "../src/server/tiger.h"

// define fixture class to setup the testing environment
class ServerApiTest: public:: testing:: Test {
 protected:
    void SetUp() override {
      Tiger::initTigerServer(App, "db.db");
      // make sure all the route handlers are in order.
      App.validate();

      // register and get a token
      req.url = "/create_account";
      App.handle_full(req, res);
      token = res.body;
      EXPECT_NE(token, "");

      // register and get another token to play the role of other clients
      req.url = "/create_account";
      App.handle_full(req, res);
      fake_token = res.body;
      EXPECT_NE(fake_token, "");

      // upload game data
      req.method = crow::HTTPMethod::POST;
      req.url = "/upload";
      req.body = "token=" + token + "&gametype=" + "RPS"
        + "&player=" + "player1" + "&result=" + "Win"
        + "&earning=" + std::to_string(100);
      App.handle_full(req, res);
      EXPECT_EQ(res.body, "SUCCESS");

      req.body = "token=" + token + "&gametype=" + "RPS"
        + "&player=" + "player2" + "&result=" + "Loss"
        + "&earning=" + std::to_string(-100);
      App.handle_full(req, res);
      EXPECT_EQ(res.body, "SUCCESS");
    }

    void TearDown() override {}

    crow::request req;
    crow::response res;
    crow::SimpleApp App;
    std::string token, fake_token;
};

TEST_F(ServerApiTest, TestGetNumberPlayers) {
  req.url = "/private/number-of-players";
  req.body = "token=" + token;
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "2");

  // request with a different token
  req.url = "/private/number-of-players";
  req.body = "token=" + fake_token;
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");
}

TEST_F(ServerApiTest, TestNumberOfGames) {
  req.url = "/private/number-of-games";
  req.body = "token=" + token + "&gametype=" + "RPS";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "2");

  // request with a different token
  req.url = "/private/number-of-games";
  req.body = "token=" + fake_token + "&gametype=" + "RPS";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");
}

TEST_F(ServerApiTest, TestEarningsAll) {
  // Test /total-earnings-all
  req.url = "/private/total-earnings-all";
  req.body = "token=" + token;
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "100");

  // request with a different token
  req.url = "/private/total-earnings-all";
  req.body = "token=" + fake_token;
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");
}

TEST_F(ServerApiTest, TestEarningsByGameAndPlayer) {
  // Test /total-earnings-game
  req.url = "/private/total-earnings-game";
  req.body = "token=" + token + "&gametype=" + "RPS";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "100");

  // Test /total-earnings-players
  req.url = "/private/total-earnings-player";
  req.body = "token=" + token + "&player=" + "player1";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "100");

  req.url = "/private/total-earnings-player";
  req.body = "token=" + token + "&player=" + "player2";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");
}

TEST_F(ServerApiTest, TestWins) {
  // Test /total-wins-all
  req.url = "/private/total-wins-all";
  req.body = "token=" + token;
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "1");

  // request with a different token
  req.url = "/private/total-wins-all";
  req.body = "token=" + fake_token;
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");
}

TEST_F(ServerApiTest, TestWinsByGame) {
  // Test /total-wins-game
  req.url = "/private/total-wins-game";
  req.body = "token=" + token + "&gametype=" + "RPS";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "1");

  // Test /total-wins-game with different gametype
  req.url = "/private/total-wins-game";
  req.body = "token=" + token + "&gametype=" + "Blackjack";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");
}

TEST_F(ServerApiTest, TestWinsByPlayer) {
  // Test /total-wins-player
  req.url = "/private/total-wins-player";
  req.body = "token=" + token + "&player=" + "player1";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "1");

  req.url = "/private/total-wins-player";
  req.body = "token=" + token + "&player=" + "player2";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");
}

TEST_F(ServerApiTest, TestLosses) {
  req.url = "/private/total-losses-all";
  req.body = "token=" + token;
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "1");

  // request with a different token
  req.url = "/private/total-losses-all";
  req.body = "token=" + fake_token;
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");
}

TEST_F(ServerApiTest, TestLossesByPlayer) {
  req.url = "/private/total-losses-player";
  req.body = "token=" + token + "&player=" + "player1";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");

  req.url = "/private/total-losses-player";
  req.body = "token=" + token + "&player=" + "player2";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "1");
}

TEST_F(ServerApiTest, TestLossesByGame) {
  req.url = "/private/total-losses-game";
  req.body = "token=" + token + "&gametype=" + "RPS";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "-100");

  // Test /total-losses-game with different gametype
  req.url = "/private/total-losses-game";
  req.body = "token=" + token + "&gametype=" + "Blackjack";
  App.handle_full(req, res);
  EXPECT_EQ(res.body, "0");
}

// TEST_F(ServerApiTest, TestMostandGreatestAPIs) {
//     // upload more game data
//     req.url = "/upload";
//     req.body = "token=" + token + "&gametype=" + "RPS"
//       + "&player=" + "player1" + "&result=" + "Win"
//       + "&earning=" + std::to_string(70);
//     App.handle_full(req, res);
//     EXPECT_EQ(res.body, "SUCCESS");

//     req.body = "token=" + token + "&gametype=" + "RPS"
//       + "&player=" + "player3" + "&result=" + "Loss"
//       + "&earning=" + std::to_string(-70);
//     App.handle_full(req, res);
//     EXPECT_EQ(res.body, "SUCCESS");

//     test greatest-player-by-wins
//     req.url = "/private/greatest-player-by-wins";
//     req.body = "token=" + token;
//     App.handle_full(req, res);
//     EXPECT_EQ(res.body, "player1");  // gives "-1"

//     req.url = "/private/most-common-play";
//     req.body = "token=" + token + "&gametype=" + "RPS";
//     App.handle_full(req, res);
//     EXPECT_EQ(res.body, "player1"); // gives "LOSS"
// }
