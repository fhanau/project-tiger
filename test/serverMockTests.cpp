#include <regex>
#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <crow.h>

#include "../src/server/auth.h"
#include "gtest/gtest.h"
#include "mockDatabase.h"

TEST(ServerMockTest, CreateNewAccount) {
  crow::SimpleApp mockApp;

  CROW_ROUTE(mockApp, "/create/<string>/<string>")
  .methods(crow::HTTPMethod::GET)
  ([](const std::string &username, const std::string &password) {
    MockDatabase sql;
    if (sql.totalMockRows("hosts", username) > 0) {
      return "ERROR";
    } else {
      std::string pw_hash = tigerAuth::get_hash(password);
      sql.insertMockData("hosts", username);
      sql.insertMockData("tokens", pw_hash);
      return "";
    }
  });

  crow::request req;
  crow::response res;

  req.url = "/create/username/password";
  mockApp.validate();
  mockApp.handle_full(req, res);
  std::string success = "";
  ASSERT_EQ(res.body, success);
}
/*
TEST(ServerMockTest, LoginExistingAccount) {

}

TEST(ServerMockTest, UploadGameData) {

}
*/
