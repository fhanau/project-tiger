#ifndef SRC_SQLITEDB_SQL_H
#define SRC_SQLITEDB_SQL_H

#include <cstdio>
#include <iostream>
#include <string>

// Simpler than including the entire sqlite3 header, which is only needed
// by sql.cpp and stat.cpp.
struct sqlite3_stmt;
struct sqlite3;


// static int textCallback(void *stringPointer, int argc, char**argv,
  // char**azColName);
// int doesExist(sqlite3_stmt* statement);

class Database {
 private:
  sqlite3* DB;
  const char* directory;
  sqlite3_stmt* the_Statement;
  int addTrigger(std::string command);

 public:
  // Create a database stored in file db_dir
  explicit Database(const char* db_dir);
  ~Database();

  // Adds a token for a new client account to the database.
  void addNewClient(const std::string& token);

  // Retrieves the user ID for a token, return -1 in case of invalid token
  int query_token(const std::string& clientToken);

  // Create an SQL table using schema described in command
  int createTable(std::string command);
  // Insert data values into SQL table
  int insertData(std::string command);
  // Select SQL data from table
  int selectData(std::string command);
  // Update data in SQL table
  int updateData(std::string command);
  // Delete entries in SQL table
  int deleteData(std::string command);
  // Drop table, given SQL command
  // int dropTable(std::string command);
  // Get the total number of rows in an SQL table
  int totalRows(std::string command);
  // Create sqlite3 statement used for processing rows
  sqlite3_stmt* makeStatement(std::string command);
  // Get the maximum value of a column in an sql table
  // int getMax(std::string table_name, std::string col_name);
  // Get the int value of a result column from an SQL query
  int getIntValue(std::string command);
  // Get the text value of a result column from an SQL query
  std::string getTextValue(std::string command);
  // Check if a query result exists in a specific table
  int doesExist(sqlite3_stmt* statement);
  // Delete a table from the database. Alex Brebenel version.
  int dropTable2(std::string command);
  // Executes an SQL command.
  int executeCommand(std::string command, std::string errMsg,
   std::string successfulMessage, int theType);
};

#endif
