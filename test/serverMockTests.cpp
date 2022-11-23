#include <regex>
#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <crow.h>

#include "../src/server/util.h"
#include "gtest/gtest.h"
#include "mockDatabase.h"

TEST(ServerMockTest, CreateNewAccount) {
  crow::SimpleApp mockApp;

  CROW_ROUTE(mockApp, "/create/<string>/<string>")
  .methods(crow::HTTPMethod::GET)
  ([](const std::string &username, const std::string &password) {
    MockDatabase sql;
    if (sql.totalMockRows("hosts", username) > 0) {
      return crow::response("ERROR");
    } else {
      std::string pw_hash = get_hash(password);
      sql.insertMockData("hosts", username);
      sql.insertMockData("tokens", pw_hash);
      return crow::response("SUCCESS");
    }
  });

  crow::request req;
  crow::response res;

  req.url = "/create/username/password";

  mockApp.handle_full(req, res);

  std::cout << res.body;

  ASSERT_EQ(res.body, "SUCCESS");
}
/*
TEST(ServerMockTest, LoginExistingAccount) {
  
}

TEST(ServerMockTest, UploadGameData) {
  
}
*/
