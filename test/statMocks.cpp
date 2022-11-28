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
  MockData theData;
  Database db = Database("dummy5.db");
  std::vector<int> result {1, 2, 3, 4, 5};

  //ON_CALL(theData,pulledIntDataVector(db, "dummyCommand"))
    //.WillByDefault(testing::Return(result));
    //(EXPECT_THAT(&db, Ref(db)))
  ON_CALL(theData, pulledIntDataVector())
    .WillByDefault(testing::Return(result));

  int med = medianValue(pulledIntDataVector(db, "dummyCommand"));
  EXPECT_EQ(med, 3);
}
