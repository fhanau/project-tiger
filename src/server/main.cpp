#include <iostream>
#include <memory>
#include <string>
#include <set>

#include "../libraries/Crow/include/crow.h"
#include "util.h"

int main(int argc, char** argv) {
  crow::SimpleApp app;

  // Create a unique session id based on the app object's location in memory
  // Give this to clients as proof they logged in successfully

  CROW_ROUTE(app, "/")([]() {
    return "Welcome to Project Tiger";
  });

  CROW_ROUTE(app, "/create/<string>/<string>").methods(crow::HTTPMethod::GET)
  ([](std::string username, std::string password) {
    std::string values = "'" + username + "', '" + password + "');";
    std::string command = "INSERT INTO hosts(username, password) VALUES(" + values;
    std::string select = "SELECT * FROM hosts;";
    getDatabase().insertData(command);
    getDatabase().selectData(select);
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

  CROW_ROUTE(app, "/gametype/<string>/<string>")([] (std::string type,
  std::string sessionId) {
    if (sessionId.compare(getSession()) != 0) {
      return "";
    }
    std::string values = "'" + type + "');";
    std::string command = "INSERT INTO games(game_name) VALUES(" + values;
    getDatabase().insertData(command);
    return "SUCCESS";
  });

  CROW_ROUTE(app, "/upload/<string>/<string>/<string>/<string>/<string>/<int>")
    ([] (std::string sessionId, std::string gametype, std::string host,
      std::string user, std::string result, std::string earning) {
    if (sessionId.compare(getSession()) != 0) {
      return "";
    }
    std::stringstream ss;
    ss << earning;

    std::string firstIns = "INSERT into game_list(game_id, game_type, ";
    std::string secondIns = "host_id, winning_player_id, result, ";
    std::string thirdIns = "money_won) VALUES(";
    std::string insert = firstIns + secondIns + thirdIns;

    std::string hostId = "HI";
    std::string firstValues = "'" + gametype + "', '" + hostId + "', '";
    std::string secondValues = user + "', '" + result + "', ";
    std::string thirdValues = ss.str() + ");";
    std::string values = firstValues + secondValues + thirdValues;

    std::string command = insert + values;
    getDatabase().insertData(command);
    return "SUCCESS";
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

  app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
