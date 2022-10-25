#ifndef SRC_SQLITEDB_SQL_H
#define SRC_SQLITEDB_SQL_H

#include <stdio.h>
#include <iostream>
#include <string>
#include "sqlite3.h"

static int callback(void* NotUsed, int argc, char** argv, char** azColName);
int doesExist(sqlite3_stmt* statement);

class Database {
  private:
    sqlite3* DB;
    const char* directory;
    sqlite3_stmt* the_Statement;

 public:
    explicit Database(const char* db_dir);
    ~Database();
    int createTable(std::string command);
    int insertData(std::string command);
    int selectData(std::string command);
    int updateData(std::string command);
    int deleteData(std::string command);
    int checkExists(std::string command);
    char *getId(std::string command);
    sqlite3_stmt* makeStatement(std::string command);
    int getMax(std::string table_name, std::string col_name);
};

#endif
