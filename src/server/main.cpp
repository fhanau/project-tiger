#include <iostream>
#include <memory>
#include <string>
#include <set>

#include "../libraries/Crow/include/crow.h"
#include "util.h"

int main(int argc, char** argv) {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([]() {
    return "Welcome to Project Tiger";
  });

  CROW_ROUTE(app, "/create/<string>/<string>").methods(crow::HTTPMethod::GET)
  ([](std::string username, std::string password) {
    std::string formattedUsername = username + "';";
    std::string findHost = "SELECT * from hosts WHERE username = '"
      + formattedUsername;
    /*sqlite3_stmt* result = getDatabase().makeStatement(findHost);*/
    if (getDatabase().checkLoginInfo(findHost)) {
      // Token required to access specific information
      return crow::response("ERROR UsernameAlreadyExists");
    } else {
      std::string values = "'" + username + "', '" + password + "');";
      std::string command = "INSERT INTO hosts(username, password) VALUES("
        + values;
      getDatabase().insertData(command);
      std::string validResponse = "SUCCESS " + getSession();
      return crow::response(validResponse);
    }
  });

  CROW_ROUTE(app, "/login/<string>/<string>")([] (std::string username,
  std::string password) {
    std::string formattedUsername = "username = '" + username + "' AND ";
    std::string formattedPassword = "password = '" + password + "';";
    std::string findHost = "SELECT * from hosts WHERE "
      + formattedUsername + formattedPassword;
    // sqlite3_stmt* result = getDatabase().makeStatement(command);
    if (!getDatabase().checkLoginInfo(findHost)) {
      // Token required to access specific information
      return crow::response("ERROR IncorrectLoginInfo");
    } else {
      std::string validResponse = "SUCCESS " + getSession();
      return crow::response(validResponse);
    }
  });

  CROW_ROUTE(app, "/gametype/<string>/<string>")([] (std::string type,
  std::string sessionId) {
    if (sessionId.compare(getSession()) != 0) {
      return crow::response("ERROR NotLoggedIn");
    }
    std::string values = "'" + type + "');";
    std::string command = "INSERT INTO games(game_name) VALUES(" + values;
    getDatabase().insertData(command);
    return crow::response("SUCCESS Complete");
  });

  CROW_ROUTE(app, "/upload/<string>/<string>/<string>/<string>/<string>/<string>")
    ([] (std::string sessionId, std::string gametype, std::string host,
      std::string user, std::string result, std::string earning) {
    if (sessionId.compare(getSession()) != 0) {
      return "";
    }

    std::string val = host + "';";
    std::string getIdCommand = "SELECT * FROM hosts WHERE username = '" + val;
    char *hostId = getDatabase().getId(getIdCommand);

    std::string firstIns = "INSERT into game_list(game_id, game_type, ";
    std::string secondIns = "host_id, winning_player_id, result, ";
    std::string thirdIns = "money_won) VALUES(";
    std::string insert = firstIns + secondIns + thirdIns;

    std::string firstValues = "'" + gametype + "', '" + hostId + "', '";
    std::string secondValues = user + "', '" + result + "', ";
    std::string thirdValues = earning + ");";
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
