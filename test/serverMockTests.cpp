#include <regex>
#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <crow.h>

#include "src/server/util.h"
#include "gtest/gtest.h"
#include "mockDatabase.h"

MockDatabase getMockDatabase() {
  static MockDatabase sql;
  return sql;
}

TEST(ServerMockTest, CreateNewAccount) {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/create/<string>/<string>").methods(crow::HTTPMethod::GET)
  ([](const std::string &username, const std::string &password) {
    std::string findHost = "SELECT * from hosts WHERE username = '" +
      username + "';";
    if (mock.totalRows(findHost) > 0) {
      return crow::response("ERROR UsernameAlreadyExists");
    } else {
      std::string pw_hash = get_hash(password);
      std::string command = "INSERT INTO hosts(username, pw_hash) VALUES("
        "'" + username + "', '" + pw_hash + "');";
      mock.insertData(command);
      std::string validResponse = "SUCCESS " + getSession();
      return crow::response(validResponse);
    }
  });

  crow::request req;
  crow::response res;
  std::unique_ptr<crow::routing_handle_result> found;

  req.url = "/create/username/password";

  app.handle(req, res, found);

  std::cout << res.body;
}

TEST(ServerMockTest, LoginExistingAccount) {
  
}

TEST(ServerMockTest, UploadGameData) {
  
}