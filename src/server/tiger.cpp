#include <iostream>
#include <string>
#include <utility>
#include <crow.h>
#include "../sqliteDB/sql.h"
#include "auth.h"
#include "tiger.h"

#define REQ_TOKEN std::string token = std::string(req.get_body_params().get("token"));
#define CROW_ROUTE_POST(app, url, impl) CROW_ROUTE(app, url).methods(crow::HTTPMethod::POST)([](const crow::request& req) impl);
  
//using Tiger::getDatabase;

Database& Tiger::getDatabase(const std::string& db_path) {
  static Database sql(db_path.c_str());
  return sql;
};

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

  CROW_ROUTE_POST(app, "/get_account_id/", {
    REQ_TOKEN;
    int user_id = tigerAuth::getAccountID(getDatabase(), token);
    if (user_id < 0) {
      return crow::response("-1");
    }
    return crow::response(std::to_string(user_id));
  })

// To facilitate having the fewest amount of code changes for now, authenticate,
// ignore the now obsolete host parameter and replace it with the user id
#define AUTHORIZED tigerAuth::getAccountID(getDatabase(), token) >= 0

  CROW_ROUTE_POST(app, "/upload", {
    // Receives request from client to upload game data to database
    // Must be logged in to upload game data
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string gametype = std::string(qs.get("type"));
    std::string hostname = std::string(qs.get("host"));
    std::string player = std::string(qs.get("player"));
    std::string result = std::string(qs.get("token"));
    std::string earning = std::string(qs.get("earning"));

    

    std::string getGamesCommand = "SELECT game_id FROM game_list;";
    int gameId = getDatabase().totalRows(getGamesCommand) + 1;
    std::string newGameId = std::to_string(gameId);

    std::string gameCommand = "INSERT INTO game_list(game_id, game_type, "
      "username, player_id, result, earning) VALUES(" +
      newGameId + ", '" + gametype + "', '" + hostname + "', '" + player +
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
      + player + "' AND username = '" + hostname + "';";
    int earningValue = std::stoi(earning);

    if (getDatabase().totalRows(playerCommand) == 0) {
      std::string newPlayerCommand = "INSERT INTO players(player_id, username)"
        " VALUES('" + player + "', '" + hostname + "');";
      getDatabase().insertData(newPlayerCommand);

      if (earningValue <= 0) {
        std::string newStatsCommand = "INSERT INTO player_stats(player_id, "
        "username, game_type, total_wins, total_losses, most_won, "
        "most_lost, total_money) VALUES('" + player + "', '" + hostname + "', '"
        + gametype + "', 0, 1, 0, " + earning + ", 0);";
        getDatabase().insertData(newStatsCommand);
      } else {
        std::string newStatsCommand = "INSERT INTO player_stats(player_id, "
        "username, game_type, total_wins, total_losses, most_won, "
        "most_lost, total_money) VALUES('" + player + "', '" + hostname + "', '"
        + gametype + "', 1, 0, " + earning + ", 0, " + earning + ");";
        getDatabase().insertData(newStatsCommand);
      }
    } else {
      if (earningValue > 0) {
        std::string mostWonCommand = "SELECT most_won FROM player_stats WHERE "
          "player_id = '" + player + "' AND username = '" + hostname + "' AND "
          "game_type = '" + gametype + "';";
        int mostWon = getDatabase().getIntValue(mostWonCommand);
        std::string totalWinsCommand = "SELECT total_wins FROM player_stats "
          "WHERE player_id = '" + player + "' AND username = '" + hostname + "' "
          "AND game_type = '" + gametype + "';";
        int totalWins = getDatabase().getIntValue(totalWinsCommand) + 1;
        std::string totalMoneyCommand = "SELECT total_money FROM player_stats "
          "WHERE player_id = '" + player + "' AND username = '" + hostname +
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
          "' AND username = '" + hostname + "' AND game_type = '" +
          gametype + "';";
        getDatabase().updateData(updateStatsCommand);
      } else {
        std::string mostLostCommand = "SELECT most_lost FROM player_stats"
          " WHERE player_id = '" + player + "' AND username = '" + hostname +
          "' AND game_type = '" + gametype + "';";
        int mostLoss = getDatabase().getIntValue(mostLostCommand);
        std::string totalLostCommand = "SELECT total_losses FROM player_stats "
          "WHERE player_id = '" + player + "' AND username = '" + hostname +
          "' AND game_type = '" + gametype + "';";
        int totalLosses = getDatabase().getIntValue(totalLostCommand) + 1;
        int newMostLoss = stoi(earning);
        if (mostLoss < newMostLoss) {
          newMostLoss = mostLoss;
        }
        std::string updateStatsCommand = "UPDATE player_stats SET total_losses"
          "= " + std::to_string(totalLosses) + ", most_lost = " +
          std::to_string(newMostLoss) + " WHERE player_id = '" + player +
          "' AND username = '" + hostname + "' AND game_type = '" + gametype + "';";
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

  CROW_ROUTE_POST(app, "/private/total-earnings-all", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string allEarningsCommand = "SELECT SUM(total_money) FROM "
      "player_stats WHERE username = '" + hostname + "';";
    return std::to_string(getDatabase().getIntValue(allEarningsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-earnings-game", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string gametype = std::string(qs.get("gametype"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string allGameEarningsCommand = "SELECT SUM(earning) FROM game_list"
      " WHERE username = '" + hostname + "' AND game_type = '" + gametype +
      "' AND earning > 0;";
    return std::to_string(getDatabase().getIntValue(allGameEarningsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-earnings-player", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string player = std::string(qs.get("player"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string totalEarnCommand = "SELECT total_money FROM "
      "player_stats WHERE username = '" + hostname + "' AND player_id = '" +
      player + "';";
    return std::to_string(getDatabase().getIntValue(totalEarnCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-wins-all", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string allWinsCommand = "SELECT SUM(total_wins) FROM player_stats"
      " WHERE username = '" + hostname + "';";
    std::cout << allWinsCommand << "\n";
    return std::to_string(getDatabase().getIntValue(allWinsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-wins-game", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string gametype = std::string(qs.get("gametype"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string allGameWinsCommand = "SELECT COUNT(game_id) FROM "
      "game_list WHERE username = '" + hostname + "' AND game_type = '" +
      gametype + "' AND earning > 0;";
    return std::to_string(getDatabase().getIntValue(allGameWinsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-wins-player", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string player = std::string(qs.get("player"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string playerWinsCommand = "SELECT total_wins FROM "
      "player_stats WHERE username = '" + hostname + "' AND player_id = '" +
      player + "';";
    return std::to_string(getDatabase().getIntValue(playerWinsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-losses-all", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string allLossesCommand = "SELECT SUM(total_losses) FROM "
      "player_stats WHERE username = '" + hostname + "';";
    return std::to_string(getDatabase().getIntValue(allLossesCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-losses-game", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string gametype = std::string(qs.get("gametype"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string allGameLossesCommand = "SELECT SUM(earning) FROM game_list "
      "WHERE username = '" + hostname + "' AND game_type = '" + gametype +
      "' AND earning <= 0;";
    return std::to_string(getDatabase().getIntValue(allGameLossesCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-losses-player", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string player = std::string(qs.get("player"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string allPlayerLossesCommand = "SELECT total_losses FROM "
      "player_stats WHERE username = '" + hostname + "' AND player_id = '" +
      player + "';";
    return std::to_string(getDatabase().getIntValue(allPlayerLossesCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-players-for-game", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string gametype = std::string(qs.get("gametype"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string totalPlayersForGameCommand = "SELECT COUNT(DISTINCT player_i"
      "d) FROM game_list WHERE game_type = '" + gametype +
      "' AND username = '" + hostname + "';";
    return std::to_string(getDatabase().getIntValue(
      totalPlayersForGameCommand));
  });

  CROW_ROUTE_POST(app, "/private/number-of-games", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string gametype = std::string(qs.get("gametype"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string numberOfGamesCommand = "";
    if (gametype == "all") {
      numberOfGamesCommand = "SELECT COUNT(DISTINCT game_id)"
      " FROM game_list WHERE username = '" + hostname + "';";
    } else {
      numberOfGamesCommand = "SELECT COUNT(DISTINCT game_id)"
      " FROM game_list WHERE game_type = '" + gametype +
      "' AND username = '" + hostname + "';";
    }
    return std::to_string(getDatabase().getIntValue(
      numberOfGamesCommand));
  });

  CROW_ROUTE_POST(app, "/private/number-of-players", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string numberOfPlayersCommand = "SELECT COUNT(*) FROM players"
      " WHERE username = '" + hostname + "';";
    return std::to_string(getDatabase().getIntValue(
      numberOfPlayersCommand));
  });

  // ALEX BREBENEL COMMENT - Might need to edit this one
 CROW_ROUTE_POST(app, "/private/greatest-player-by-wins", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string greatestPlayerByWinsCommand = "SELECT player_id, "
      "SUM(total_wins) AS tw FROM player_stats WHERE "
      "username = '" + hostname + "' AND tw = (SELECT MAX(total_wins) "
      "FROM player_stats WHERE username = '" + hostname + "'"
      " GROUP BY player_id) GROUP BY player_id;";

    return getDatabase().getTextValue(greatestPlayerByWinsCommand);
  });

  CROW_ROUTE_POST(app, "/private/most-common-play", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string gametype = std::string(qs.get("gametype"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string findGame = "SELECT * from game_list WHERE username = '" +
      hostname + "' AND game_type = '" + gametype + "';";

    if (getDatabase().totalRows(findGame) == 0) {
      return std::string("GameDataNotFound");
    }
    std::string mostCommonPlayCommand = "SELECT result, COUNT(result) AS "
      "'value_occurrence' FROM game_list WHERE username = '" + hostname +
      "' AND game_type = '" + gametype + "' GROUP BY result ORDER BY "
      "'value_occurence' DESC LIMIT 1;";
    return getDatabase().getTextValue(mostCommonPlayCommand);
  });

  CROW_ROUTE_POST(app, "/private/most-winning-play", {
    crow::query_string qs = req.get_body_params();
    std::string token = std::string(qs.get("token"));
    std::string hostname = std::string(qs.get("hostname"));
    std::string gametype = std::string(qs.get("gametype"));
    if (!AUTHORIZED) {
      return std::string("ERROR:NotAuthenticated");
    }
    std::string findGame = "SELECT * from game_list WHERE username = '" +
      hostname + "' AND game_type = '" + gametype + "';";

    if (getDatabase().totalRows(findGame) == 0) {
      return std::string("GameDataNotFound");
    }
    std::string mostWinningPlayCommand = "SELECT result, MAX(theCount)"
      " FROM (SELECT result, COUNT(result) AS theCount FROM game_list WHERE "
      "earning > 0 AND username = '" + hostname + "' AND game_type = '" +
      gametype + "' GROUP BY result);";
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