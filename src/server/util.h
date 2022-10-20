#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <memory>
#include <string>
#include <set>

int isValidTypeOfPublicRequest(std::string type);
std::string requestPublicGameData();
std::string handlePublicRequest(std::string type);
std::string getSession();

#endif