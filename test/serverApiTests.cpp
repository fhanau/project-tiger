#include "gtest/gtest.h"
#include "../src/server/util.h"

TEST(ServerUtilTest, ReturnsUniqueSession) {
  std::string sessionId = getSession();
  EXPECT_GE(sessionId.size(), 1);

  std::string firstTwoCharsOfSessionId = sessionId.substr(0, 2);
  std::string memoryLocationPointerPrefix = "0x";
  ASSERT_EQ(firstTwoCharsOfSessionId, memoryLocationPointerPrefix);
}

TEST(ServerUtilTest, ReturnsSameUniqueSession) {
  std::string sessionIdFirstCall = getSession();
  std::string sessionIdSecondCall = getSession();
  ASSERT_EQ(sessionIdFirstCall, sessionIdSecondCall);
}
