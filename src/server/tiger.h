#ifndef TIGER_H_
#define TIGER_H_
#include <string>
#include "../sqliteDB/sql.h"

namespace Tiger {
  // Returns singleton class for Database
  Database getDatabase(const std::string& db_path = "db.db");

  // Run the program for Project Tiger and use the file at db_path as the
  // database backend. Create the database file if needed.
  void runTigerServer(const std::string& db_path);
}
#endif // TIGER_H_
