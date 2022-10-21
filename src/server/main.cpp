#include <iostream>
#include <memory>
#include <string>
#include <set>

#include "../libraries/Crow/include/crow.h"
#include "util.h"

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

int main(int argc, char** argv) {
  crow::SimpleApp app;

  // Create a unique session id based on the app object's location in memory
  // Give this to clients as proof they logged in successfully

  CROW_ROUTE(app, "/")([]() {
    return "Welcome to Project Tiger";
  });

  CROW_ROUTE(app, "/create/<string>/<string>").methods(crow::HTTPMethod::GET)
  ([](std::string username, std::string password) {
    std::string hostId = gen_random(15);
    std::string values = "'"+ hostId + "', '" + username + "', '" + password + "');";
    std::string command = "INSERT INTO hosts(host_id, username, password) VALUES(" + values;
    getDatabase().insertData(command);
    return crow::response(getSession());
  });

  CROW_ROUTE(app, "/login/<string>/<string>")([] (std::string username,
  std::string password) {
    int isValidLogin = 1;
    if (isValidLogin) {
      // Token required to access specific information
      return crow::response(getSession());
    } else {
      return crow::response("");
    }
  });

  CROW_ROUTE(app, "/public/<string>")([] (std::string type) {
    crow::json::wvalue resp({{"type", ""}});
    if (isValidTypeOfPublicRequest(type)) {
      resp["type"] = "ERROR";
      resp["errorMessage"] = "Invalid type of request for data.";
      return resp;
    }
    // Function for processing valid type
    std::string result = handlePublicRequest(type);
    if (!result.compare("ERROR")) {
      resp["type"] = "ERROR";
      resp["errorMessage"] = "Error when requesting public game data.";
      return resp;
    }
    resp["type"] = "public";
    resp["data"] = result;
    return resp;
  });

  CROW_ROUTE(app, "/gametype/<string>/<string>")([] (std::string type,
  std::string sessionId) {
    if (sessionId.compare(getSession()) != 0) {
      return "";
    }
    Database sql("../data/db.db");
    std::string values = "'" + type + "');";
    std::string command = "INSERT INTO games(game_type) VALUES(" + values;
    getDatabase().insertData(command);
    return "SUCCESS";
  });

  app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
