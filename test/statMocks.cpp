#include <array>
#include <vector>
#include <sqlite3.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../src/sqliteDB/sql.h"
#include "../src/sqliteDB/stat.h"


class MockDatabase : public Database {
 public:
  MockDatabase() : Database("dummy5.db") {}
   // Causes crashes, temporarily disabled
   // MOCK_METHOD(std::vector<int>, pulledIntDataVector, (const std::string&),
   // (override));
   std::vector<int> override pulledIntDataVector(const std::string& sql_command) {
     std::vector<int> mock_result {1, 2, 3, 4, 5};
     return mock_result;
   }
};

#define DUMMY_CMD "SELECT total_wins, player_id FROM player_stats  WHERE " \
  "username = 'username101' ORDER BY total_wins ASC;"
TEST(MedianMock, gettingMockMedian) {
  MockDatabase db;
  // EXPECT_CALL(db, pulledIntDataVector(testing::_)).Times(testing::AtLeast(1)).WillOnce(testing::Return(result));
  // ON_CALL(db, pulledIntDataVector).WillByDefault(testing::Return(result));
  std::vector<int> test = db.pulledIntDataVector(DUMMY_CMD);
  int med = medianValue(test);
  EXPECT_EQ(med, 3);
}

TEST(BoxWhiskerMock, getMockBoxStats) {
  MockDatabase db;
  // EXPECT_CALL(db, pulledIntDataVector(testing::_)).Times(testing::AtLeast(1));
  // ON_CALL(db, pulledIntDataVector).WillByDefault(testing::Return(result));
  std::vector<int> test = db.pulledIntDataVector(DUMMY_CMD);
  std::array<float, 4> boxStats = percentileValues(test);

  EXPECT_EQ(boxStats.at(0), 1.5);
  EXPECT_EQ(boxStats.at(1), 3);
  EXPECT_EQ(boxStats.at(2), 4.5);
  EXPECT_EQ(boxStats.at(3), 3);
}

TEST(BoxWhiskerMockEmpty, getMockBoxStatsEmpty) {
  MockDatabase db;
  std::vector<int> result {};
  // EXPECT_CALL(db, pulledIntDataVector(testing::_)).Times(testing::AtLeast(1));
  // ON_CALL(db, pulledIntDataVector).WillByDefault(testing::Return(result));

  std::vector<int> test = db.pulledIntDataVector(DUMMY_CMD);
  std::array<float, 4> boxStats = percentileValues(test);

  EXPECT_EQ(boxStats.at(0), 1.5);
  EXPECT_EQ(boxStats.at(1), 3);
  EXPECT_EQ(boxStats.at(2), 4.5);
  EXPECT_EQ(boxStats.at(3), 3);
}
