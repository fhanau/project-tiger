#include "gtest/gtest.h"
#include "../src/server/util.h"
#include "../src/sqliteDB/sql.h"

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

TEST(ServerUtilTest, ReturnsSameDatabase) {
  Database databaseFirstCall = getDatabase();
  Database databaseSecondCall = getDatabase();
  Database* firstCallPointer = &databaseFirstCall;
  Database* secondCallPointer = &databaseSecondCall;
  ASSERT_EQ(databaseFirstCall, databaseSecondCall);
}
