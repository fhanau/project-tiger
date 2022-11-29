#include <array>
#include <vector>
#include <sqlite3.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../src/sqliteDB/sql.h"
#include "../src/sqliteDB/stat.h"


class MockData {
 public:
    MOCK_METHOD(std::vector<int>, pulledIntDataVector,
      (Database& db, const std::string& sql_command), ());
};

TEST(MedianMock, gettingMockMedian) {
  testing::NiceMock<MockData> theData;
  Database db = Database("dummy5.db");
  std::vector<int> result {1, 2, 3, 4, 5};

  ON_CALL(theData, pulledIntDataVector)
    .WillByDefault(testing::Return(result));

  std::vector<int> test = theData.pulledIntDataVector(db, "dummyCom");
  int med = medianValue(test);
  EXPECT_EQ(med, 3);
}

TEST(BoxWhiskerMock, getMockBoxStats) {
  testing::NiceMock<MockData> theData;
  Database db = Database("dummy5.db");
  std::vector<int> result {1, 2, 3, 4};

  ON_CALL(theData, pulledIntDataVector)
    .WillByDefault(testing::Return(result));

  std::vector<int> test = theData.pulledIntDataVector(db, "dummyCom");
  std::array<float, 4> boxStats = percentileValues(test);

  EXPECT_EQ(boxStats.at(0), 1.5);
  EXPECT_EQ(boxStats.at(1), 2.5);
  EXPECT_EQ(boxStats.at(2), 3.5);
  EXPECT_EQ(boxStats.at(3), 2.0);
}

TEST(BoxWhiskerMockEmpty, getMockBoxStatsEmpty) {
  testing::NiceMock<MockData> theData;
  Database db = Database("dummy5.db");
  std::vector<int> result {};

  ON_CALL(theData, pulledIntDataVector)
    .WillByDefault(testing::Return(result));

  std::vector<int> test = theData.pulledIntDataVector(db, "dummyCom");
  std::array<float, 4> boxStats = percentileValues(test);

  EXPECT_EQ(boxStats.at(0), 0.0);
  EXPECT_EQ(boxStats.at(1), 0.0);
  EXPECT_EQ(boxStats.at(2), 0.0);
  EXPECT_EQ(boxStats.at(3), 0.0);
}
