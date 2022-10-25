#include <iostream>
#include <memory>
#include <string>
#include <set>

#include "util.h"
#include "../sqliteDB/sql.h"

Database getDatabase() {
  static Database sql("db.db");
  return sql;
}

std::string getSession() {
  static std::string session = "";
  if (session.size() > 0) {
    return session;
  }
  int randomLocationInMemory = 1000;
  char randomLocationAsLiteral[9];
  snprintf(randomLocationAsLiteral, sizeof(randomLocationAsLiteral),
  "%p", &randomLocationInMemory);
  session = std::string(randomLocationAsLiteral);
  return session;
}
