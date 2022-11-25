#include <iostream>
#include <string>
#include <utility>
#include <crow.h>
#include "../sqliteDB/sql.h"
#include "auth.h"
#include "tiger.h"

using Tiger::getDatabase;

Database& Tiger::getDatabase(const std::string& db_path) {
  static Database sql(db_path.c_str());
  return sql;
}

// Initializes server used for clients to connect to service
void Tiger::initTigerServer(crow::SimpleApp& app, const std::string& db_path) {
  CROW_ROUTE(app, "/")([]() {
    return "Welcome to Project Tiger";
  });

  CROW_ROUTE(app, "/create_account")([]() {
    std::string new_token = tigerAuth::createUniqueToken(getDatabase());
    int new_user_id = tigerAuth::getAccountID(getDatabase(), new_token);
    std::string pw_hash = tigerAuth::get_hash(new_token);
    std::string command = "INSERT INTO hosts(username, pw_hash) VALUES("
      "'" + std::to_string(new_user_id) + "', '" + pw_hash + "');";
    getDatabase().insertData(command);
    return crow::response(new_token);
  });

  CROW_ROUTE(app, "/get_account_id/<string>")([] (const std::string &token) {
    int user_id = tigerAuth::getAccountID(getDatabase(), token);
    if (user_id < 0) {
      return crow::response("Request failed: Could not authenticate using "
          "token");
    }
    return crow::response(std::to_string(user_id));
  });

// To facilitate having the fewest amount of code changes for now, authenticate,
// ignore the now obsolete host parameter and replace it with the user id
#define AUTH int user_id = tigerAuth::getAccountID(getDatabase(), token); \
  if (user_id < 0) { \
    return (std::string)"Request failed: Could not authenticate using token"; \
  } \
  std::string host = std::to_string(user_id);
  /*if (host != std::to_string(user_id)) {
    return "Request failed: Not authorized to access data";
  }*/

  CROW_ROUTE(app, "/upload/<string>/<string>/<string>/<string>/"
    "<string>/<string>")([] (const std::string &token,
    const std::string &gametype, const std::string &_host,
    const std::string &player, const std::string &result,
    const std::string &earning) {
    // Receives request from client to upload game data to database
    // Must be logged in to upload game data
    AUTH;

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
    return (std::string)"SUCCESS";
  });

  CROW_ROUTE(app, "/public/<string>")([] (const std::string &type) {
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
    ([] (const std::string &token, const std::string &_host) {
     AUTH;
      std::string allEarningsCommand = "SELECT SUM(total_money) FROM "
        "player_stats WHERE username = '" + host + "';";
      return std::to_string(getDatabase().getIntValue(allEarningsCommand));
  });

  CROW_ROUTE(app, "/private/total-earnings-game/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &gametype) {
      AUTH;
      std::string allGameEarningsCommand = "SELECT SUM(earning) FROM game_list"
        " WHERE username = '" + host + "' AND game_type = '" + gametype +
        "' AND earning > 0;";
      return std::to_string(getDatabase().getIntValue(allGameEarningsCommand));
  });

  CROW_ROUTE(app, "/private/total-earnings-player/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &player) {
      AUTH;
      std::string totalEarnCommand = "SELECT total_money FROM "
        "player_stats WHERE username = '" + host + "' AND player_id = '" +
        player + "';";
      return std::to_string(getDatabase().getIntValue(totalEarnCommand));
  });

  CROW_ROUTE(app, "/private/total-wins-all/<string>/<string>")
    ([] (const std::string &token, const std::string &_host) {
      AUTH;
      std::string allWinsCommand = "SELECT SUM(total_wins) FROM player_stats"
        " WHERE username = '" + host + "';";
      std::cout << allWinsCommand << "\n";
      return std::to_string(getDatabase().getIntValue(allWinsCommand));
  });

  CROW_ROUTE(app, "/private/total-wins-game/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &gametype) {
      AUTH;
      std::string allGameWinsCommand = "SELECT COUNT(game_id) FROM "
        "game_list WHERE username = '" + host + "' AND game_type = '" +
        gametype + "' AND earning > 0;";
      return std::to_string(getDatabase().getIntValue(allGameWinsCommand));
  });

  CROW_ROUTE(app, "/private/total-wins-player/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &player) {
      AUTH;
      std::string playerWinsCommand = "SELECT total_wins FROM "
        "player_stats WHERE username = '" + host + "' AND player_id = '" +
        player + "';";
      return std::to_string(getDatabase().getIntValue(playerWinsCommand));
  });

  CROW_ROUTE(app, "/private/total-losses-all/<string>/<string>")
    ([] (const std::string &token, const std::string &_host) {
      AUTH;
      std::string allLossesCommand = "SELECT SUM(total_losses) FROM "
        "player_stats WHERE username = '" + host + "';";
      return std::to_string(getDatabase().getIntValue(allLossesCommand));
  });

  // input[0]/input[1]/token (constant)/ host (constant) / input[2].....
  CROW_ROUTE(app, "/private/total-losses-game/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &gametype) {
      AUTH;
      std::string allGameLossesCommand = "SELECT SUM(earning) FROM game_list "
        "WHERE username = '" + host + "' AND game_type = '" + gametype +
        "' AND earning <= 0;";
      return std::to_string(getDatabase().getIntValue(allGameLossesCommand));
  });

  CROW_ROUTE(app, "/private/total-players-for-game/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &gametype) {
      AUTH;
      std::string totalPlayersForGameCommand = "SELECT COUNT(DISTINCT player_i"
        "d) FROM game_list WHERE game_type = '" + gametype +
        "' AND username = '" + host + "';";
      return std::to_string(getDatabase().getIntValue(
        totalPlayersForGameCommand));
  });

  CROW_ROUTE(app, "/private/number-of-games/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &gametype) {
      AUTH;
      std::string numberOfGamesCommand = "";
      if (gametype == "all") {
        numberOfGamesCommand = "SELECT COUNT(DISTINCT game_id)"
        " FROM game_list WHERE username = '" + host + "';";
      } else {
        numberOfGamesCommand = "SELECT COUNT(DISTINCT game_id)"
        " FROM game_list WHERE game_type = '" + gametype +
        "' AND username = '" + host + "';";
      }
      return std::to_string(getDatabase().getIntValue(
        numberOfGamesCommand));
  });

  CROW_ROUTE(app, "/private/number-of-players/<string>/<string>")
    ([] (const std::string &token, const std::string &_host) {
      AUTH;
      std::string numberOfPlayersCommand = "SELECT COUNT(*) FROM players"
        " WHERE username = '" + host + "';";
      return std::to_string(getDatabase().getIntValue(
        numberOfPlayersCommand));
  });

  // ALEX BREBENEL COMMENT - Might need to edit this one
  CROW_ROUTE(app, "/private/greatest-player-by-wins/<string>/<string>")
    ([] (const std::string &token, const std::string &_host) {
      AUTH;
      std::string greatestPlayerByWinsCommand = "SELECT player_id, "
        "SUM(total_wins) AS tw FROM player_stats WHERE "
        "username = '" + host + "' AND tw = (SELECT MAX(total_wins) "
        "FROM player_stats WHERE username = '" + host + "'"
        " GROUP BY player_id) GROUP BY player_id;";

      return getDatabase().getTextValue(greatestPlayerByWinsCommand);
  });

  CROW_ROUTE(app, "/private/total-losses-player/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &player) {
      AUTH;
      std::string allPlayerLossesCommand = "SELECT total_losses FROM "
        "player_stats WHERE username = '" + host + "' AND player_id = '" +
        player + "';";
      return std::to_string(getDatabase().getIntValue(allPlayerLossesCommand));
  });

  CROW_ROUTE(app, "/private/most-common-play/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &gametype) {
      AUTH;
      std::string findGame = "SELECT * from game_list WHERE username = '" +
        host + "' AND game_type = '" + gametype + "';";

      if (getDatabase().totalRows(findGame) == 0) {
        return std::string("GameDataNotFound");
      }
      std::string mostCommonPlayCommand = "SELECT result, COUNT(result) AS "
        "'value_occurrence' FROM game_list WHERE username = '" + host +
        "' AND game_type = '" + gametype + "' GROUP BY result ORDER BY "
        "'value_occurence' DESC LIMIT 1;";
      return getDatabase().getTextValue(mostCommonPlayCommand);
  });

  CROW_ROUTE(app, "/private/most-winning-play/<string>/<string>/<string>")
    ([] (const std::string &token, const std::string &_host,
    const std::string &gametype) {
      AUTH;
      std::string findGame = "SELECT * from game_list WHERE username = '" +
        host + "' AND game_type = '" + gametype + "';";

      if (getDatabase().totalRows(findGame) == 0) {
        return std::string("GameDataNotFound");
      }
      std::string mostWinningPlayCommand = "SELECT result, MAX(theCount)"
        " FROM (SELECT result, COUNT(result) AS theCount FROM game_list WHERE "
        "earning > 0 AND username = '" + host + "' AND game_type = '" +
        gametype + "' GROUP BY result);";
      //std::cout << mostWinningPlayCommand << "\n";
      return getDatabase().getTextValue(mostWinningPlayCommand);
  });

  //Set up SSL, working around Crow issues
  crow::ssl_context_t ssl_ctx(asio::ssl::context::sslv23);
  ssl_ctx.set_verify_mode(asio::ssl::verify_none);
  ssl_ctx.use_certificate_file("cert.pem", crow::ssl_context_t::pem);
  ssl_ctx.use_private_key_file("key.pem", crow::ssl_context_t::pem);
  ssl_ctx.set_options(
      asio::ssl::context::default_workarounds
      | asio::ssl::context::no_sslv2
      | asio::ssl::context::no_sslv3);
  app.ssl(std::move(ssl_ctx));

  app.port(18080).multithreaded().run();
}
