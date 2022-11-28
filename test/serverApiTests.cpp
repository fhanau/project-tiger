#include <regex>
#include <typeinfo>
#include <crow.h>
#include "gtest/gtest.h"
#include "../src/server/tiger.h"
#include "../src/server/auth.h"

TEST(ServerUtilTest, ReturnsValidToken) {
  Database db("dummy.db");
  std::string sessionId = tigerAuth::createUniqueToken(db);
  EXPECT_EQ(sessionId.size(), TOKEN_BASE64_LEN);

  // Check if session ID contains characters not allowed in url-safe base64
  // Inspired by https://stackoverflow.com/a/7616973
  bool contains_non_base64
    = !std::regex_match(sessionId, std::regex("^[A-Za-z0-9\\-_\\=]+$"));
  ASSERT_FALSE(contains_non_base64);
}

TEST(ServerUtilTest, ReturnsUniqueSession) {
  Database db("dummy.db");
  std::string sessionIdFirstCall = tigerAuth::createUniqueToken(db);
  std::string sessionIdSecondCall = tigerAuth::createUniqueToken(db);
  ASSERT_NE(sessionIdFirstCall, sessionIdSecondCall);
}

TEST(ServerUtilTest, ReturnsDistinctIDs) {
  Database db("dummy.db");
  std::string sessionIdFirstCall = tigerAuth::createUniqueToken(db);
  std::string sessionIdSecondCall = tigerAuth::createUniqueToken(db);
  int accountID1 = tigerAuth::getAccountID(db, sessionIdFirstCall);
  int accountID2 = tigerAuth::getAccountID(db, sessionIdSecondCall);
  ASSERT_NE(accountID1, accountID2);
}

TEST(ServerUtilTest, ReturnsCorrectHashForPassword) {
  std::string hashedPassword1 = tigerAuth::get_hash("password");
  std::string hashedPassword2 = tigerAuth::get_hash("password");
  ASSERT_EQ(hashedPassword1, hashedPassword2);
}

TEST(ServerUtilTest, ReturnsUniqueHashForDifferentPasswords) {
  std::string hashedPassword1 = tigerAuth::get_hash("password1");
  std::string hashedPassword2 = tigerAuth::get_hash("password2");
  ASSERT_NE(hashedPassword1, hashedPassword2);
}

// TEST(ServerUtilTest, ReturnsDatabase) {
//   Database database = Tiger::getDatabase();
//   std::string databaseType = typeid(database).name();
//   std::string type = "Database";
//   int correctType = databaseType.find(type) != std::string::npos;
//   ASSERT_EQ(correctType, 1);
// }

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
