#ifndef SRC_SERVER_UTIL_H_
#define SRC_SERVER_UTIL_H_
#include <iostream>
#include <memory>
#include <string>
#include <set>
#include "../sqliteDB/sql.h"

// Returns singleton class for Database
Database getDatabase();

// Returns unique memory location as string to client
// Serves as token for verifying that client is logged in
std::string getSession();

#endif // SRC_SERVER_UTIL_H_
