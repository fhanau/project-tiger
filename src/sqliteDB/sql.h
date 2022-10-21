#include <stdio.h>
#include <iostream>
#include "../libraries/sqlite/sqlite3.h"

class Database {
  private:
    sqlite3* DB;
    const char* directory;

 public:
    explicit Database(const char* db_dir);
    ~Database();
    int createTable(std::string command);
    int insertData(std::string command);
    int selectData(std::string command);
    int updateData(std::string command);
    int deleteData(std::string command);
};