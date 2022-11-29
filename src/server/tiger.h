#ifndef TIGER_H_
#define TIGER_H_
#include <string>
#include <crow/app.h>
#include "../sqliteDB/sql.h"

namespace Tiger {
  // Returns singleton class for Database
  Database& getDatabase(const std::string& db_path = "db.db");

  // Initialize the RESTful server for Project Tiger without starting it. Use
  // the file at db_path as the database backend. Create the database file if
  // needed.
  void initTigerServer(crow::SimpleApp& app, const std::string& db_path);
}
#endif  // TIGER_H_
