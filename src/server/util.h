#ifndef SRC_SERVER_UTIL_H_
#define SRC_SERVER_UTIL_H_
#include <iostream>
#include <memory>
#include <string>
#include <set>

int isValidTypeOfPublicRequest(std::string type);
std::string requestPublicGameData();
std::string handlePublicRequest(std::string type);
std::string getSession();

#endif // SRC_SERVER_UTIL_H_
