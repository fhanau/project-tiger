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
    std::string findHost = "SELECT * from hosts WHERE username = '" +
      username + "';";
    if (getDatabase().totalRows(findHost) > 0) {
      return crow::response("ERROR UsernameAlreadyExists");
    } else {
      std::string command = "INSERT INTO hosts(username, password) VALUES("
        "'" + username + "', '" + password + "');";
      getDatabase().insertData(command);
      std::string validResponse = "SUCCESS " + getSession();
      return crow::response(validResponse);
    }
  });

  CROW_ROUTE(app, "/login/<string>/<string>")([] (std::string username,
  std::string password) {
    // Receives request from client to login with username and password
    std::string findHost = "SELECT * from hosts WHERE username = '" + username
      + "' AND password = '" + password + "';";
    if (getDatabase().totalRows(findHost) == 0) {
      return crow::response("ERROR IncorrectLoginInfo");
    } else {
      std::string validResponse = "SUCCESS " + getSession();
      return crow::response(validResponse);
    }
  });

  CROW_ROUTE(app, "/upload/<string>/<string>/<string>/<string>/<string>/<string>")
    ([] (std::string sessionId, std::string gametype, std::string host,
      std::string player, std::string result, std::string earning) {
    // Receives request from client to upload game data to database
    // Must be logged in to upload game data
    if (sessionId.compare(getSession()) != 0) {
      return "";
    }

    std::string getGamesCommand = "SELECT game_id FROM game_list;";
    int totalGames = getDatabase().totalRows(getGamesCommand);
    std::cout << "Total Games Played: " << totalGames << "\n";
    int gameId = totalGames + 1;
    
    std::string newGameId = std::to_string(gameId);

    std::string gameCommand = "INSERT INTO game_list(game_id, game_type, "
      "username, winning_player_id, result, money_won) VALUES(" + 
      newGameId + ", '" + gametype + "', '" + host + "', '" + player + 
      "', '" + result + "', " + earning + ");";

    std::string gameTypeCommand = "SELECT * FROM games WHERE "
      "game_name = '" + gametype + "';";
    int totalGameTypes = getDatabase().totalRows(gameTypeCommand);
    if (totalGameTypes == 0) {
      std::string newGameTypeCommand = "INSERT INTO games(game_name) VALUES('"
        + gametype + "');";
      getDatabase().insertData(newGameTypeCommand);
    }

    std::string playerCommand = "SELECT * FROM players WHERE player_id = '"
      + player + "' AND username = '" + host + "';";
    
    if (getDatabase().totalRows(playerCommand) == 0) {
      std::string newPlayerCommand = "INSERT INTO players(player_id, username)"
        " VALUES('" + player + "', '" + host + "');";
      getDatabase().insertData(newPlayerCommand);

      // Need to change tests since "name" field should be removed
      // Also how would we know how much that lost or total losses?
      std::string newStatsCommand = "INSERT INTO player_stats(player_id, "
        "username, game_type, total_wins, total_losses, most_won, "
        "most_lost, total_money) VALUES('" + player + "', '" + host + "', '"
        + gametype + "', 1, 0, " + earning + ", 0, " + earning + ");";
      getDatabase().insertData(newStatsCommand);
    } else {
      std::string mostWonCommand = "SELECT most_won FROM player_stats WHERE "
        "player_id = '" + player + "' AND username = '" + host + "' AND "
        "game_type = '" + gametype + "';";
      int mostWon = getDatabase().getMostWon(mostWonCommand);
      int newMostWon = mostWon ? mostWon > stoi(earning) : stoi(earning);
      std::string updateStatsCommand = "UPDATE player_stats SET total_wins = "
        "total_wins + 1, most_won = " + std::to_string(newMostWon) +
        ", total_money = total_money + " + earning + " WHERE player_id = '"
        + player + "' AND username = '" + host + "', AND game_type = '" +
        gametype + "';";
      getDatabase().updateData(updateStatsCommand);
    }
    getDatabase().insertData(gameCommand);
    return "SUCCESS";
  });

  CROW_ROUTE(app, "/public/<string>")([] (std::string type) {
    if (!type.compare("total-games")) {
      std::string command = "SELECT game_id FROM game_list;";
      return std::to_string(getDatabase().totalRows(command));
    } else if (!type.compare("total-hosts")) {
      std::string command = "SELECT (username) FROM hosts;";
      return std::to_string(getDatabase().totalRows(command));
    } else if (!type.compare("total-players")) {
      std::string command = "SELECT (player_id) FROM players;";
      return std::to_string(getDatabase().totalRows(command));
    } else if (!type.compare("total-types")) {
      std::string command = "SELECT (game_name) FROM games;";
      return std::to_string(getDatabase().totalRows(command));
    } else {
      return std::to_string(0);
    }
  });

  app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
