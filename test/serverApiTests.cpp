#include <regex>
#include "gtest/gtest.h"
#include "../src/server/util.h"

TEST(ServerUtilTest, ReturnsUniqueSession) {
  std::string sessionId = getSession();
  EXPECT_EQ(sessionId.size(), TOKEN_BASE64_LEN);

  //Check if session ID contains characters not allowed in base64
  //Inspired by https://stackoverflow.com/a/7616973
  bool contains_non_base64
    = !std::regex_match(sessionId, std::regex("^[A-Za-z0-9\\+\\/\\=]+$"));
  ASSERT_FALSE(contains_non_base64);
}

TEST(ServerUtilTest, ReturnsSameUniqueSession) {
  std::string sessionIdFirstCall = getSession();
  std::string sessionIdSecondCall = getSession();
  ASSERT_EQ(sessionIdFirstCall, sessionIdSecondCall);
}

TEST(ServerUtilTest, ReturnsCorrectHashForPassword) {
  std::string hashedPassword1 = get_hash("password");
  std::string hashedPassword2 = get_hash("password");
  ASSERT_EQ(hashedPassword1, hashedPassword2);
}

TEST(ServerUtilTest, ReturnsUniqueHashForDifferentPasswords) {
  std::string hashedPassword1 = get_hash("password1");
  std::string hashedPassword2 = get_hash("password2");
  ASSERT_NE(hashedPassword1, hashedPassword2);
}

TEST(ServerUtilTest, ReturnsSameDatabase) {
  Database databaseFirstCall = getDatabase();
  Database databaseSecondCall = getDatabase();
  ASSERT_EQ(&databaseFirstCall, &databaseSecondCall);
}