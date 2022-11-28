#include <sqlite3.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../src/sqliteDB/sql.h"
#include "../src/sqliteDB/stat.h"


class MockData {
  public:
    MOCK_METHOD(std::vector<int>, pulledIntDataVector,
      (Database& db, const std::string& sql_command), ());
    MOCK_METHOD(std::vector<int>, pulledIntDataVector,
      (), ());
};

TEST(MedianMock, gettingMockMedian) {
  MockData theData;
  Database db = Database("dummy5.db");
  std::vector<int> result {1,2,3,4,5};

  //ON_CALL(theData,pulledIntDataVector(db, "dummyCommand"))
    //.WillByDefault(testing::Return(result));
    //(EXPECT_THAT(&db, Ref(db)))
  ON_CALL(theData, pulledIntDataVector())
    .WillByDefault(testing::Return(result));

  int med = medianValue(pulledIntDataVector(db, "dummyCommand"));
  EXPECT_EQ(med, 3);
}



/*



class MockDatabase2 : public Database {
 private:
  sqlite3* DB;
  const char* directory;
  sqlite3_stmt* the_Statement;
  int addTrigger(std::string command);

 public:
  // Create a database stored in file db_dir
  //explicit Database(const char* db_dir);
  //~Database();

  //MOCK_METHOD(int, createTable, (std::string));
  MOCK_METHOD(sqlite3_stmt*, makeStatement, (std::string), ());

  sqlite3_stmt* dummy;

  // sqlite3_stmt* makeStatement(std::string command);

};


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
    POST_INIT_GAMETYPE;
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
    POST_INIT_GAMETYPE;
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
    std::string greatestPlayerByWinsCommand = "SELECT player_id, "
      "SUM(total_wins) AS tw FROM player_stats WHERE "
      "username = '" + acct_id + "' AND tw = (SELECT MAX(total_wins) "
      "FROM player_stats WHERE username = '" + acct_id + "'"
      " GROUP BY player_id) GROUP BY player_id;";

    return getDatabase().getTextValue(greatestPlayerByWinsCommand);
  });

  CROW_ROUTE_POST(app, "/private/most-common-play", {
    POST_INIT_GAMETYPE;
    std::string findGame = "SELECT * from game_list WHERE username = '" +
      acct_id + "' AND game_type = '" + gametype + "';";

    if (getDatabase().totalRows(findGame) == 0) {
      return std::string("GameDataNotFound");
    }
    std::string mostCommonPlayCommand = "SELECT result, COUNT(result) AS "
      "'value_occurrence' FROM game_list WHERE username = '" + acct_id +
      "' AND game_type = '" + gametype + "' GROUP BY result ORDER BY "
      "'value_occurence' DESC LIMIT 1;";
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
  
  */