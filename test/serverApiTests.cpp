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
<<<<<<< HEAD
}

TEST(ServerUtilTest, ReturnsSameDatabase) {
  Database databaseFirstCall = getDatabase();
  Database databaseSecondCall = getDatabase();
  char firstLocation[9];
  snprintf(firstLocation, sizeof(firstLocation), "%p", &databaseFirstCall);
  std::string firstMemoryLocationLiteral = std::string(firstLocation);
  char secondLocation[9];
  snprintf(secondLocation, sizeof(secondLocation), "%p", &databaseSecondCall);
  std::string secondMemoryLocationLiteral = std::string(secondLocation);
  ASSERT_EQ(firstMemoryLocationLiteral, secondMemoryLocationLiteral);
=======
>>>>>>> 58b8acd3fea19e9b4dadfe926c3e0667896b16b0
}
