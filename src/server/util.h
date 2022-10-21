#ifndef SRC_SERVER_UTIL_H_
#define SRC_SERVER_UTIL_H_
#include <iostream>
#include <memory>
#include <string>
#include <set>
#include "../sqliteDB/sql.h"

std::string gen_random(const int len);
Database getDatabase();
int isValidTypeOfPublicRequest(std::string type);
std::string requestPublicGameData();
std::string handlePublicRequest(std::string type);
std::string getSession();

#endif // SRC_SERVER_UTIL_H_
