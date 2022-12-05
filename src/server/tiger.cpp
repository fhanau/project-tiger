#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <crow/app.h>
#include "../sqliteDB/sql.h"
#include "auth.h"
#include "tiger.h"
#include "../sqliteDB/stat.h"

#define CROW_ROUTE_POST(app, url, impl) CROW_ROUTE(app, url).methods( \
    crow::HTTPMethod::POST)([](const crow::request& req) impl);

Database& Tiger::getDatabase(const std::string& db_path) {
  static Database sql(db_path.c_str());
  return sql;
}

// Retrieve the token from the url string. Note that the token is guaranteed to
// end with a single '=' due to the base64 encoding. This gets cut off by the
// query string processing, so we need to add it back.
#define GET_TOKEN \
  const crow::query_string qs = req.get_body_params(); \
  if (!qs.get("token")) { \
    return std::string("ERROR:MissingAuthToken");\
  } \
  std::string token = std::string(qs.get("token")) + "=";

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
    return new_token;
  });

  CROW_ROUTE_POST(app, "/get_account_id", {
    GET_TOKEN;
    int acct_id = tigerAuth::getAccountID(getDatabase(), token);
    if (acct_id < 0) {
      return (std::string)"-1";
    }
    return std::to_string(acct_id);
  });

// Shared setup and parsing code for most private functions
#define POST_INIT \
  GET_TOKEN \
  int _acct_id = tigerAuth::getAccountID(getDatabase(), token); \
  if (_acct_id < 0) { \
    return std::string("ERROR:NotAuthenticated"); \
  } \
  std::string acct_id = std::to_string(_acct_id);

// Same as above, also reads gametype parameter
#define POST_INIT_GAMETYPE \
  POST_INIT; \
  if (!qs.get("gametype")) { \
    return std::string("ERROR:MissingGametype");\
  } \
  std::string gametype = std::string(qs.get("gametype"));

#define GET_PLAYER \
  if (!qs.get("player")) { \
    return std::string("ERROR:MissingPlayer");\
  } \
  std::string player = std::string(qs.get("player"));

  CROW_ROUTE_POST(app, "/upload", {
    // Receives request from client to upload game data to database
    // Must be logged in to upload game data
    POST_INIT_GAMETYPE;
    GET_PLAYER;

    if (!qs.get("result") || !qs.get("earning")) {
      return std::string("ERROR:MissingParameter");
    }
    std::string result = std::string(qs.get("result"));
    std::string earning = std::string(qs.get("earning"));

    std::string getGamesCommand = "SELECT game_id FROM game_list;";
    int gameId = getDatabase().totalRows(getGamesCommand) + 1;
    std::string newGameId = std::to_string(gameId);

    std::string gameCommand = "INSERT INTO game_list(game_id, game_type, "
      "username, player_id, result, earning) VALUES(" +
      newGameId + ", '" + gametype + "', '" + acct_id + "', '" + player +
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
      + player + "' AND username = '" + acct_id + "';";
    int earningValue = std::stoi(earning);

    if (getDatabase().totalRows(playerCommand) == 0) {
      std::string newPlayerCommand = "INSERT INTO players(player_id, username)"
        " VALUES('" + player + "', '" + acct_id + "');";
      getDatabase().insertData(newPlayerCommand);

      if (earningValue <= 0) {
        std::string newStatsCommand = "INSERT INTO player_stats(player_id, "
        "username, game_type, total_wins, total_losses, most_won, "
        "most_lost, total_money) VALUES('" + player + "', '" + acct_id + "', '"
        + gametype + "', 0, 1, 0, " + earning + ", 0);";
        getDatabase().insertData(newStatsCommand);
      } else {
        std::string newStatsCommand = "INSERT INTO player_stats(player_id, "
        "username, game_type, total_wins, total_losses, most_won, "
        "most_lost, total_money) VALUES('" + player + "', '" + acct_id + "', '"
        + gametype + "', 1, 0, " + earning + ", 0, " + earning + ");";
        getDatabase().insertData(newStatsCommand);
      }
    } else {
      if (earningValue > 0) {
        std::string mostWonCommand = "SELECT most_won FROM player_stats WHERE "
          "player_id = '" + player + "' AND username = '" + acct_id + "' AND "
          "game_type = '" + gametype + "';";
        int mostWon = getDatabase().getIntValue(mostWonCommand);
        std::string totalWinsCommand = "SELECT total_wins FROM player_stats "
          "WHERE player_id = '" + player + "' AND username = '" + acct_id + "' "
          "AND game_type = '" + gametype + "';";
        int totalWins = getDatabase().getIntValue(totalWinsCommand) + 1;
        std::string totalMoneyCommand = "SELECT total_money FROM player_stats "
          "WHERE player_id = '" + player + "' AND username = '" + acct_id +
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
          "' AND username = '" + acct_id + "' AND game_type = '" +
          gametype + "';";
        getDatabase().updateData(updateStatsCommand);
      } else {
        std::string mostLostCommand = "SELECT most_lost FROM player_stats"
          " WHERE player_id = '" + player + "' AND username = '" + acct_id +
          "' AND game_type = '" + gametype + "';";
        int mostLoss = getDatabase().getIntValue(mostLostCommand);
        std::string totalLostCommand = "SELECT total_losses FROM player_stats "
          "WHERE player_id = '" + player + "' AND username = '" + acct_id +
          "' AND game_type = '" + gametype + "';";
        int totalLosses = getDatabase().getIntValue(totalLostCommand) + 1;
        int newMostLoss = stoi(earning);
        if (mostLoss < newMostLoss) {
          newMostLoss = mostLoss;
        }
        std::string updateStatsCommand = "UPDATE player_stats SET total_losses"
          "= " + std::to_string(totalLosses) + ", most_lost = " +
          std::to_string(newMostLoss) + " WHERE player_id = '" + player +
          "' AND username = '" + acct_id + "' AND game_type = '" + gametype +
          "';";
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
    POST_INIT;
    std::string allEarningsCommand = "SELECT SUM(total_money) FROM "
      "player_stats WHERE username = '" + acct_id + "';";
    return std::to_string(getDatabase().getIntValue(allEarningsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-earnings-game", {
    POST_INIT_GAMETYPE;
    std::string allGameEarningsCommand = "SELECT SUM(earning) FROM game_list"
      " WHERE username = '" + acct_id + "' AND game_type = '" + gametype +
      "' AND earning > 0;";
    return std::to_string(getDatabase().getIntValue(allGameEarningsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-earnings-player", {
    POST_INIT;
    GET_PLAYER;
    std::string totalEarnCommand = "SELECT total_money FROM "
      "player_stats WHERE username = '" + acct_id + "' AND player_id = '" +
      player + "';";
    return std::to_string(getDatabase().getIntValue(totalEarnCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-wins-all", {
    POST_INIT;
    std::string allWinsCommand = "SELECT SUM(total_wins) FROM player_stats"
      " WHERE username = '" + acct_id + "';";
    return std::to_string(getDatabase().getIntValue(allWinsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-wins-game", {
    POST_INIT_GAMETYPE;
    std::string allGameWinsCommand = "SELECT COUNT(game_id) FROM "
      "game_list WHERE username = '" + acct_id + "' AND game_type = '" +
      gametype + "' AND earning > 0;";
    return std::to_string(getDatabase().getIntValue(allGameWinsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-wins-player", {
    POST_INIT;
    GET_PLAYER;
    std::string playerWinsCommand = "SELECT total_wins FROM "
      "player_stats WHERE username = '" + acct_id + "' AND player_id = '" +
      player + "';";
    return std::to_string(getDatabase().getIntValue(playerWinsCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-losses-all", {
    POST_INIT;
    std::string allLossesCommand = "SELECT SUM(total_losses) FROM "
      "player_stats WHERE username = '" + acct_id + "';";
    return std::to_string(getDatabase().getIntValue(allLossesCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-losses-game", {
    POST_INIT_GAMETYPE;
    std::string allGameLossesCommand = "SELECT SUM(earning) FROM game_list "
      "WHERE username = '" + acct_id + "' AND game_type = '" + gametype +
      "' AND earning <= 0;";
    return std::to_string(getDatabase().getIntValue(allGameLossesCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-losses-player", {
    POST_INIT;
    GET_PLAYER;
    std::string allPlayerLossesCommand = "SELECT total_losses FROM "
      "player_stats WHERE username = '" + acct_id + "' AND player_id = '" +
      player + "';";
    return std::to_string(getDatabase().getIntValue(allPlayerLossesCommand));
  });

  CROW_ROUTE_POST(app, "/private/total-players-for-game", {
    POST_INIT_GAMETYPE;
    std::string totalPlayersForGameCommand = "SELECT COUNT(DISTINCT player_i"
      "d) FROM game_list WHERE game_type = '" + gametype +
      "' AND username = '" + acct_id + "';";
    return std::to_string(getDatabase().getIntValue(
      totalPlayersForGameCommand));
  });

  CROW_ROUTE_POST(app, "/private/number-of-games", {
    POST_INIT_GAMETYPE;
    std::string numberOfGamesCommand = "";
    if (gametype == "all") {
      numberOfGamesCommand = "SELECT COUNT(DISTINCT game_id)"
      " FROM game_list WHERE username = '" + acct_id + "';";
    } else {
      numberOfGamesCommand = "SELECT COUNT(DISTINCT game_id)"
      " FROM game_list WHERE game_type = '" + gametype +
      "' AND username = '" + acct_id + "';";
    }
    return std::to_string(getDatabase().getIntValue(
      numberOfGamesCommand));
  });

  CROW_ROUTE_POST(app, "/private/number-of-players", {
    POST_INIT;
    std::string numberOfPlayersCommand = "SELECT COUNT(*) FROM players"
      " WHERE username = '" + acct_id + "';";
    return std::to_string(getDatabase().getIntValue(
      numberOfPlayersCommand));
  });

  // ALEX BREBENEL COMMENT - Might need to edit this one
  CROW_ROUTE_POST(app, "/private/greatest-player-by-wins", {
    POST_INIT;
    std::string greatestPlayerByWinsCommand = "SELECT player_id, MAX(wins) "
      "FROM (SELECT player_id, COUNT(player_id) as wins FROM game_list "
      "WHERE earning > 0 AND username = '" + acct_id + "' "
      "GROUP BY player_id);";
    
    /*"SELECT player_id, "
      "SUM(total_wins) AS tw FROM player_stats WHERE "
      "username = '" + acct_id + "' AND tw = (SELECT MAX(total_wins) "
      "FROM player_stats WHERE username = '" + acct_id + "'"
      " GROUP BY player_id) GROUP BY player_id;";
*/
    return getDatabase().getTextValue(greatestPlayerByWinsCommand);
  });

  CROW_ROUTE_POST(app, "/private/most-common-play", {
    POST_INIT_GAMETYPE;
    std::string findGame = "SELECT * from game_list WHERE username = '" +
      acct_id + "' AND game_type = '" + gametype + "';";

    if (getDatabase().totalRows(findGame) == 0) {
      return std::string("GameDataNotFound");
    }
    std::string mostCommonPlayCommand = "SELECT result, MAX(theCount)"
      " FROM (SELECT result, COUNT(result) AS theCount FROM game_list WHERE "
      "username = '" + acct_id + "' AND game_type = '" +
      gametype + "' GROUP BY result);";
    return getDatabase().getTextValue(mostCommonPlayCommand);
  });

  CROW_ROUTE_POST(app, "/private/most-winning-play", {
    POST_INIT_GAMETYPE;
    std::string findGame = "SELECT * from game_list WHERE username = '" +
      acct_id + "' AND game_type = '" + gametype + "';";

    if (getDatabase().totalRows(findGame) == 0) {
      return std::string("GameDataNotFound");
    }
    std::string mostWinningPlayCommand = "SELECT result, MAX(theCount)"
      " FROM (SELECT result, COUNT(result) AS theCount FROM game_list WHERE "
      "earning > 0 AND username = '" + acct_id + "' AND game_type = '" +
      gametype + "' GROUP BY result);";
    return getDatabase().getTextValue(mostWinningPlayCommand);
  });

  CROW_ROUTE_POST(app, "/private/total-player-earning-for-game", {
    POST_INIT_GAMETYPE;
    GET_PLAYER;
    std::string command = "SELECT SUM(earning) FROM game_list "
      "WHERE username = '" + acct_id + "' AND game_type = '" + gametype +
      "' AND playerid = '" + player + "' AND earning >= 0;";
    return std::to_string(getDatabase().getIntValue(command));
  });

  CROW_ROUTE_POST(app, "/private/total-player-debt-for-game", {
    POST_INIT_GAMETYPE;
    GET_PLAYER;
    std::string command = "SELECT SUM(earning) FROM game_list "
      "WHERE username = '" + acct_id + "' AND game_type = '" + gametype +
      "' AND playerid = '" + player + "' AND earning < 0;";
    return std::to_string(getDatabase().getIntValue(command));
  });

  CROW_ROUTE_POST(app, "/private/total-player-wins-for-game", {
    POST_INIT_GAMETYPE;
    GET_PLAYER;
    std::string command = "SELECT * FROM game_list "
      "WHERE username = '" + acct_id + "' AND game_type = '" + gametype +
      "' AND playerid = '" + player + "' AND earning >= 0;";
    return std::to_string(getDatabase().totalRows(command));
  });

  CROW_ROUTE_POST(app, "/private/total-player-losses-for-game", {
    POST_INIT_GAMETYPE;
    GET_PLAYER;
    std::string command = "SELECT * FROM game_list "
      "WHERE username = '" + acct_id + "' AND game_type = '" + gametype +
      "' AND playerid = '" + player + "' AND earning < 0;";
    return std::to_string(getDatabase().totalRows(command));
  });

  CROW_ROUTE_POST(app, "/private/most-winning-player-for-game", {
    POST_INIT_GAMETYPE;

    std::string findGame = "SELECT * from game_list WHERE username = '" +
      acct_id + "' AND game_type = '" + gametype + "';";
    if (getDatabase().totalRows(findGame) == 0) {
      return std::string("GameDataNotFound");
    }
    std::string command = "SELECT player_id, MAX(theCount)"
      " FROM (SELECT player_id, COUNT(player_id) AS theCount FROM game_list"
      " WHERE earning > 0 AND username = '" + acct_id + "' AND game_type = '" +
      gametype + "' GROUP BY player_id);";
    return std::to_string(getDatabase().getIntValue(command));
  });

  CROW_ROUTE_POST(app, "/private/median-earning", {
    POST_INIT_GAMETYPE;

    std::string findGame = "SELECT * from game_list WHERE username = '" +
      acct_id + "' AND game_type = '" + gametype + "';";
    if (getDatabase().totalRows(findGame) == 0) {
      return std::string("GameDataNotFound");
    }
    std::string command = "SELECT earning FROM game_list"
      " WHERE username = '" + acct_id + "' AND game_type = '" + gametype +
      "' ORDER BY earning ASC";
    std::vector<int> values = pulledIntDataVector(getDatabase(), command);
    return std::to_string(medianValue(values));
  });

  // Set up SSL, working around Crow issues
  crow::ssl_context_t ssl_ctx(asio::ssl::context::sslv23);
  ssl_ctx.set_verify_mode(asio::ssl::verify_none);
  ssl_ctx.use_certificate_file("cert.pem", crow::ssl_context_t::pem);
  ssl_ctx.use_private_key_file("key.pem", crow::ssl_context_t::pem);
  ssl_ctx.set_options(
      asio::ssl::context::default_workarounds
      | asio::ssl::context::no_sslv2
      | asio::ssl::context::no_sslv3);
  app.ssl(std::move(ssl_ctx));
}
