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

  CROW_ROUTE(app, "/upload/<string>/<string>/<string>/<string>/"
    "<string>/<string>")([] (std::string sessionId, std::string gametype,
    std::string host, std::string player, std::string result,
    std::string earning) {
    // Receives request from client to upload game data to database
    // Must be logged in to upload game data
    if (sessionId.compare(getSession()) != 0) {
      return "";
    }

    std::string getGamesCommand = "SELECT game_id FROM game_list;";
    int gameId = getDatabase().totalRows(getGamesCommand) + 1;
    std::string newGameId = std::to_string(gameId);

    std::string gameCommand = "INSERT INTO game_list(game_id, game_type, "
      "username, player_id, result, earning) VALUES(" +
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
    int earningValue = std::stoi(earning);

    if (getDatabase().totalRows(playerCommand) == 0) {
      std::string newPlayerCommand = "INSERT INTO players(player_id, username)"
        " VALUES('" + player + "', '" + host + "');";
      getDatabase().insertData(newPlayerCommand);

      if (earningValue <= 0) {
        std::string newStatsCommand = "INSERT INTO player_stats(player_id, "
        "username, game_type, total_wins, total_losses, most_won, "
        "most_lost, total_money) VALUES('" + player + "', '" + host + "', '"
        + gametype + "', 0, 1, 0, " + earning + ", 0);";
        getDatabase().insertData(newStatsCommand);
      } else {
        std::string newStatsCommand = "INSERT INTO player_stats(player_id, "
        "username, game_type, total_wins, total_losses, most_won, "
        "most_lost, total_money) VALUES('" + player + "', '" + host + "', '"
        + gametype + "', 1, 0, " + earning + ", 0, " + earning + ");";
        getDatabase().insertData(newStatsCommand);
      }
    } else {
      if (earningValue > 0) {
        std::string mostWonCommand = "SELECT most_won FROM player_stats WHERE "
          "player_id = '" + player + "' AND username = '" + host + "' AND "
          "game_type = '" + gametype + "';";
        int mostWon = getDatabase().getIntValue(mostWonCommand);
        std::string totalWinsCommand = "SELECT total_wins FROM player_stats "
          "WHERE player_id = '" + player + "' AND username = '" + host + "' "
          "AND game_type = '" + gametype + "';";
        int totalWins = getDatabase().getIntValue(totalWinsCommand) + 1;
        std::string totalMoneyCommand = "SELECT total_money FROM player_stats "
          "WHERE player_id = '" + player + "' AND username = '" + host +
          "' AND game_type = '" + gametype + "';";
        int totalMoney = getDatabase().getIntValue(totalMoneyCommand) +
          stoi(earning);
        int newMostWon = stoi(earning);
        if (mostWon > newMostWon) {
          newMostWon = mostWon;
        }
        std::string updateStatsCommand = "UPDATE player_stats SET total_wins"
          " = " + std::to_string(totalWins) + ", most_won = " +
          std::to_string(newMostWon) + ", total_money = " +
          std::to_string(totalMoney) + " WHERE player_id = '" + player +
          "' AND username = '" + host + "' AND game_type = '" +
          gametype + "';";
        getDatabase().updateData(updateStatsCommand);
      } else {
        std::string mostLostCommand = "SELECT most_lost FROM player_stats"
          " WHERE player_id = '" + player + "' AND username = '" + host +
          "' AND game_type = '" + gametype + "';";
        int mostLoss = getDatabase().getIntValue(mostLostCommand);
        std::string totalLostCommand = "SELECT total_losses FROM player_stats "
          "WHERE player_id = '" + player + "' AND username = '" + host +
          "' AND game_type = '" + gametype + "';";
        int totalLosses = getDatabase().getIntValue(totalLostCommand) + 1;
        int newMostLoss = stoi(earning);
        if (mostLoss < newMostLoss) {
          newMostLoss = mostLoss;
        }
        std::string updateStatsCommand = "UPDATE player_stats SET total_losses"
          "= " + std::to_string(totalLosses) + ", most_lost = " +
          std::to_string(newMostLoss) + " WHERE player_id = '" + player +
          "' AND username = '" + host + "' AND game_type = '" + gametype + "';";
        getDatabase().updateData(updateStatsCommand);
      }
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

  CROW_ROUTE(app, "/private/total-earnings-all/<string>/<string>")
    ([] (std::string session, std::string host) {
      if (getSession().compare(session)) {
        return std::to_string(-1);
      }
      std::string allEarningsCommand = "SELECT SUM(total_money) FROM "
        "player_stats WHERE username = '" + host + "';";
      return std::to_string(getDatabase().getIntValue(allEarningsCommand));
  });

  CROW_ROUTE(app, "/private/total-earnings-game/<string>/<string>/<string>")
    ([] (std::string session, std::string host, std::string gametype) {
      if (getSession().compare(session)) {
        return std::to_string(-1);
      }
      std::string allGameEarningsCommand = "SELECT SUM(earning) FROM game_list"
        " WHERE username = '" + host + "' AND game_type = '" + gametype +
        "' AND earning > 0;";
      return std::to_string(getDatabase().getIntValue(allGameEarningsCommand));
  });

  CROW_ROUTE(app, "/private/total-earnings-player/<string>/<string>/<string>")
    ([] (std::string session, std::string host, std::string player) {
      if (getSession().compare(session)) {
        return std::to_string(-1);
      }
      std::string totalEarnCommand = "SELECT total_money FROM "
        "player_stats WHERE username = '" + host + "' AND player_id = '" +
        player + "';";
      return std::to_string(getDatabase().getIntValue(totalEarnCommand));
  });

  CROW_ROUTE(app, "/private/total-wins-all/<string>/<string>")
    ([] (std::string session, std::string host) {
      if (getSession().compare(session)) {
        return std::to_string(-1);
      }
      std::string allWinsCommand = "SELECT SUM(total_wins) FROM player_stats"
        " WHERE username = '" + host + "';";
      std::cout << allWinsCommand << "\n";
      return std::to_string(getDatabase().getIntValue(allWinsCommand));
  });

  CROW_ROUTE(app, "/private/total-wins-game/<string>/<string>/<string>")
    ([] (std::string session, std::string host, std::string gametype) {
      if (getSession().compare(session)) {
        return std::to_string(-1);
      }
      std::string allGameWinsCommand = "SELECT COUNT(game_id) FROM "
        "game_list WHERE username = '" + host + "' AND game_type = '" +
        gametype + "' AND earning > 0;";
      return std::to_string(getDatabase().getIntValue(allGameWinsCommand));
  });

  CROW_ROUTE(app, "/private/total-wins-player/<string>/<string>/<string>")
    ([] (std::string session, std::string host, std::string player) {
      if (getSession().compare(session)) {
        return std::to_string(-1);
      }
      std::string playerWinsCommand = "SELECT total_wins FROM "
        "player_stats WHERE username = '" + host + "' AND player_id = '" +
        player + "';";
      return std::to_string(getDatabase().getIntValue(playerWinsCommand));
  });

  CROW_ROUTE(app, "/private/total-losses-all/<string>/<string>")
    ([] (std::string session, std::string host) {
      if (getSession().compare(session)) {
        return std::to_string(-1);
      }
      std::string allLossesCommand = "SELECT SUM(total_losses) FROM "
        "player_stats WHERE username = '" + host + "';";
      return std::to_string(getDatabase().getIntValue(allLossesCommand));
  });

  CROW_ROUTE(app, "/private/total-losses-game/<string>/<string>/<string>")
    ([] (std::string session, std::string host, std::string gametype) {
      if (getSession().compare(session)) {
        return std::to_string(-1);
      }
      std::string allGameLossesCommand = "SELECT SUM(earning) FROM game_list "
        "WHERE username = '" + host + "' AND game_type = '" + gametype +
        "' AND earning <= 0;";
      return std::to_string(getDatabase().getIntValue(allGameLossesCommand));
  });

  CROW_ROUTE(app, "/private/total-losses-player/<string>/<string>/<string>")
    ([] (std::string session, std::string host, std::string player) {
      if (getSession().compare(session)) {
        return std::to_string(-1);
      }
      std::string allPlayerLossesCommand = "SELECT total_losses FROM "
        "player_stats WHERE username = '" + host + "' AND player_id = '" +
        player + "';";
      return std::to_string(getDatabase().getIntValue(allPlayerLossesCommand));
  });

  CROW_ROUTE(app, "/private/most-common-play/<string>/<string>/<string>")
    ([] (std::string session, std::string host, std::string gametype) {
      if (getSession().compare(session)) {
        return std::string("Invalid sessionid. Logout and login again.\n");
      }
      std::string mostCommonPlayCommand = "SELECT result, COUNT(result) AS "
        "'value_occurrence' FROM game_list WHERE username = '" + host +
        "' AND game_type = '" + gametype + "' GROUP BY result ORDER BY "
        "'value_occurence' DESC LIMIT 1;";
      return getDatabase().getTextValue(mostCommonPlayCommand);
  });

  CROW_ROUTE(app, "/private/most-winning-play/<string>/<string>/<string>")
    ([] (std::string session, std::string host, std::string gametype) {
      if (getSession().compare(session)) {
        return std::string("Invalid sessionid. Logout and login again.\n");
      }
      std::string mostWinningPlayCommand = "SELECT result, MAX(theCount)"
        " FROM (SELECT result, COUNT(result) AS 'theCount' game_list WHERE "
        "earning > 0 AND username = '" + host + "' AND game_type = '" +
        gametype + "' GROUP BY result)";
      return getDatabase().getTextValue(mostWinningPlayCommand);
  });

  app.port(18080).multithreaded().run();
}
