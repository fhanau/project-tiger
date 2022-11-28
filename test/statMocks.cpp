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
  MockData theData;
  Database db = Database("dummy5.db");
  std::vector<int> result {1, 2, 3, 4, 5};

  ON_CALL(theData, pulledIntDataVector)
    .WillByDefault(testing::Return(result));

  int med = medianValue(theData.pulledIntDataVector(db, "dummyCommand"));
  EXPECT_EQ(med, 3);
}
