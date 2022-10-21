#include <iostream>
#include <memory>
#include <string>
#include <set>

#include "util.h"
#include "../sqliteDB/sql.h"

std::string gen_random(const int len) {
  //Lifted from:
  //https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  std::string tmp_s;
  tmp_s.reserve(len);

  for (int i = 0; i < len; ++i) {
      tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }
  
  return tmp_s;
}

Database getDatabase() {
  static Database sql("db.db");
  return sql;
}
int isValidTypeOfPublicRequest(std::string type) {
  std::set<std::string> validTypes = {"games", "total"};
  return validTypes.find(type) != validTypes.end();
}

std::string requestPublicGameData() {
  // Can simplify by just having very basic information and one query for it
  // Can expand to a couple of different types of public data requests
  // Call SQLWrapper.getPublicGameData(); regardless
  // If an error occurs when quering SQL, return ERROR
  int isError = 0;
  if (isError) {
    return "ERROR";
  }
  // Would be set equal to some formatted version of the SQL data table result.
  std::string publicData = "Public Information: We crunch numbers :)";
  return publicData;
}

std::string handlePublicRequest(std::string type) {
  if (!type.compare("games")) {
    return requestPublicGameData();
  }
  return "";
}

std::string getSession() {
  static std::string session = "";
  if (session.size() > 0) {
    return session;
  }
  int randomLocationInMemory = 1000;
  char randomLocationAsLiteral[9];
  snprintf(randomLocationAsLiteral, sizeof(randomLocationAsLiteral), "%p", &randomLocationInMemory);
  session = std::string(randomLocationAsLiteral);
  return session;
}
