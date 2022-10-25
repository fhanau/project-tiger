#include <iostream>
#include <memory>
#include <string>
#include <set>

#include "../libraries/Crow/include/crow.h"
#include "util.h"

int main(int argc, char** argv) {
  // Runs server for clients to connect to service
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([]() {
    return "Welcome to Project Tiger";
  });

  CROW_ROUTE(app, "/create/<string>/<string>").methods(crow::HTTPMethod::GET)
  ([](std::string username, std::string password) {
    // Receives request from client to make account with username and password
    std::string formattedUsername = username + "';";
    std::string findHost = "SELECT * from hosts WHERE username = '"
      + formattedUsername;
    if (getDatabase().totalRows(findHost) > 0) {
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
    // Receives request from client to login with username and password
    std::string formattedUsername = "username = '" + username + "' AND ";
    std::string formattedPassword = "password = '" + password + "';";
    std::string findHost = "SELECT * from hosts WHERE "
      + formattedUsername + formattedPassword;
    if (getDatabase().totalRows(findHost) == 0) {
      return crow::response("ERROR IncorrectLoginInfo");
    } else {
      std::string validResponse = "SUCCESS " + getSession();
      return crow::response(validResponse);
    }
  });

  CROW_ROUTE(app, "/gametype/<string>/<string>")([] (std::string type,
  std::string sessionId) {
    // Receives request from client to upload new type of game
    // Must have sessionId to verify that client is logged in
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
    // Receives request from client to upload game data to database
    // Must be logged in to upload game data
    if (sessionId.compare(getSession()) != 0) {
      return "";
    }

    int gameId = 0;
    std::string getGamesCommand = "SELECT * FROM game_list LIMIT 1";
    if (getDatabase().totalRows(getGamesCommand) > 0) {
      gameId = getDatabase().getMax("game_list", "game_id") + 1;
    }
    std::string newGameId = std::to_string(gameId);

    std::string firstIns = "INSERT INTO game_list(game_id, game_type, ";
    std::string secondIns = "username, winning_player_id, result, ";
    std::string thirdIns = "money_won) VALUES(";
    std::string insert = firstIns + secondIns + thirdIns;

    std::string firstValues = newGameId + ", '" + gametype + "', '";
    std::string secondValues = host + "', '" + user + "', '" + result;
    std::string thirdValues = "', " + earning + ");";
    std::string values = firstValues + secondValues + thirdValues;

    std::string command = insert + values;
    std::cout << command << "\n";
    getDatabase().insertData(command);
    return "SUCCESS";
  });

  CROW_ROUTE(app, "/public/<string>")([] (std::string type) {
    if (!type.compare("total-games")) {
      std::string command = "SELECT * FROM game_list;";
      int totalGames = getDatabase().totalRows(command);
      return std::to_string(totalGames);
    }
    return std::to_string("-1");
  });

  app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
