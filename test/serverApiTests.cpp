#include "gtest/gtest.h"
#include "../src/server/tiger.h"

TEST(ServerApiTest, TestGetNumberPlayers) {
  crow::SimpleApp mockApp;
  Tiger::initTigerServer(mockApp, "db.db");
  mockApp.validate();   //Used to make sure all the route handlers are in order.
  crow::request req;
  crow::response res;

  req.url = "/create_account";
  req.raw_url = "/create_account";
  mockApp.handle_full(req, res);
  std::string token = res.body;
  EXPECT_NE(token, "");

  req.method = crow::HTTPMethod::POST;
  req.url = "/private/number-of-players";
  req.body = "token=" + token;
  mockApp.handle_full(req, res);
  std::string success = "0";
  EXPECT_EQ(res.body, success);
}
