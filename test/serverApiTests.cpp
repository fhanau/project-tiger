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
