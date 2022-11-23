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
      std::string command = "INSERT INTO hosts(username, pw_hash) VALUES("
        "'" + username + "', '" + pw_hash + "');";
      sql.insertMockData("hosts", username);
      sql.insertMockData("tokens", pw_hash);
      return crow::response("SUCCESS");
    }
  });

  crow::request req;
  crow::response res;
  std::unique_ptr<crow::routing_handle_result> found;

  req.url = "/create/username/password";

  mockApp.handle(req, res, found);

  std::cout << res.body;

  ASSERT_EQ(res.body, "SUCCESS");
}
/*
TEST(ServerMockTest, LoginExistingAccount) {
  
}

TEST(ServerMockTest, UploadGameData) {
  
}
*/
