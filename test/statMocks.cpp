#include <sqlite3.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../src/sqliteDB/sql.h"
#include "../src/sqliteDB/stat.h"


class MockData {
 public:
    MOCK_METHOD(std::vector<int>, pulledIntDataVector,
      (Database& db, const std::string& sql_command), ());
    MOCK_METHOD(std::vector<int>, pulledIntDataVector,
      (), ());
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
