#ifndef TEST_MOCKDATABASE_H
#define TEST_MOCKDATABASE_H

#include <string>
#include "gmock/gmock.h"
#include "src/sqliteDB/sql.h"

class MockDatabase : public Database {
 public:
  // Mock creating a database table
  MOCK_METHOD(void, createTable, (std::string command), (override));
  // Mock inserting into a database table
  MOCK_METHOD(int, insertData, (std::string command), (override));
  // Mock selecting data from a database table
  MOCK_METHOD(int, selectData, (std::string command), (override));
  // Mock updating data in a database table
  MOCK_METHOD(int, updateData, (std::string command), (override));
  // Mock deleting data in a database table
  MOCK_METHOD(int, deleteData, (std::string command), (override));
  // Mock dropping a database table
  MOCK_METHOD(int, dropTable, (std::string command), (override));
  // Mock finding the total rows in a database table
  MOCK_METHOD(int, totalRows, (std::string command), (override));
  // Mock making a sqlite3 statement
  MOCK_METHOD(sqlite3_stmt*, makeStatement, (std::string command), (override));
  // Mock getting the max value for a column in a database table
  MOCK_METHOD(int, getMax, (std::string table_name, std::string col_name), (override));
  // Mock getting an int value from a database table
  MOCK_METHOD(int, getIntValue, (std::string command), (override));
  // Mock getting a text value from a database table
  MOCK_METHOD(std::string, getTextValue, (std::string command), (override));
  // Mock checking if a value exists in a table
  MOCK_METHOD(int, doesExist, (sqlite3_stmt* statement), (override));
  // Mock dropping a database table
  MOCK_METHOD(int, dropTable2, (std::string command), (override));
  // Mock executing a command
  MOCK_METHOD(int, executeCommand, (std::string command, std::string errMsg,
   std::string successfulMessage, int theType), (override));
};

#endif