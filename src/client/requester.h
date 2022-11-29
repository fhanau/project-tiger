#ifndef SRC_CLIENT_REQUESTER_H
#define SRC_CLIENT_REQUESTER_H

#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"

// Handles making all GET requests to server using input from client
class Requester {
 public:
  Requester() {
    reset();
  }

  // Sends GET request to server to create a new token for this client
  std::vector<std::string> createToken();

  // Sends GET request to server to retrieve the id of a token
  std::string getTokenID(const std::string &token);

  // Sends GET request to server to upload data and returns server response
  std::vector<std::string> uploadGameData(const std::string &token,
   const std::string &type, const std::string &user,
   const std::string &result, const std::string &earning);

  // Sends GET request to server for publicly available statistics
  std::string getPublicStats(const std::string &type);

  // Sends POST request to server for total earnings across all games
  std::string getTotalEarningsAll(const std::string &token);

  // Sends POST request to server for total earnings for a game
  std::string getTotalEarningsGame(const std::string &token,
    const std::string &gametype);

  // Sends POST request to server for total earnings for a player
  std::string getTotalEarningsPlayer(const std::string &token,
    const std::string &player);

  // Sends POST request to server for total wins across all games
  std::string getTotalWinsAll(const std::string &token);

  // Sends POST request to server for total wins for a game
  std::string getTotalWinsGame(const std::string &token,
    const std::string &gametype);

  // Sends POST request to server for total wins for a &
  std::string getTotalWinsPlayer(const std::string &token,
    const std::string &player);

  // Sends POST request to server for total losses across all games
  std::string getTotalLossesAll(const std::string &token);

  // Sends POST request to server for total losses for a game
  std::string getTotalLossesGame(const std::string &token,
    const std::string &gametype);

  // Sends POST request to server for total losses for a player
  std::string getTotalLossesPlayer(const std::string &token,
    const std::string &player);

  // Sends POST request to server for most common result for a game
  std::string getMostCommonPlay(const std::string &token,
    const std::string &gametype);

  // Sends POST request to server for most common result for winners
  std::string getMostWinningPlay(const std::string &token,
    const std::string &gametype);

  // Sends POST request to server for total losses for a player
  std::string getTotalPlayersForGame(const std::string &token,
    const std::string &gametype);

  // ALEX BREBENEL COMMENT Might need to fix
  // Sends POST request to server for best player by number of wins
  std::string getGreatestPlayerByWins(const std::string &token);

  // Sends POST request to server for number of results
  std::string getNumberOfResults(const std::string& token,
    const std::string& gametype);

  // Sends POST request to server for number of players
  std::string getNumberOfPlayers(const std::string& token);

  // Sends POST request to server for total earnings for a player
  std::string getTotalPlayerEarningsForGame(const std::string& token,
    const std::string& gametype, const std::string& player);

  // Sends POST request to server for total debt for a player
  std::string getTotalPlayerDebtForGame(const std::string& token,
    const std::string& gametype, const std::string& player);

  // Sends POST request to server for total wins for a player
  std::string getTotalPlayerWinsForGame(const std::string& token,
    const std::string& gametype, const std::string& player);

  // Sends POST request to server for total lossing for a player
  std::string getTotalPlayerLossesForGame(const std::string& token,
    const std::string& gametype, const std::string& player);

  // Sends POST request to server for most winning player
  std::string getMostWinningPlayerForGame(const std::string& token,
    const std::string& gametype);

  // Sends POST request to server for median earning
  std::string getMedianEarning(const std::string& token,
    const std::string& gametype);

  // Alex Brebenel streamline request function
  std::string theRequester(const std::string& path, const std::string& body);

 private:
  // reset the internal curlpp::Easy instance and set up SSL
  void reset();

  // Perform a GET or POST request using the request object and url and write
  // the response to payload.
  void perform(std::vector<std::string>& payload, const std::string& url);

  curlpp::Cleanup cleaner;
  curlpp::Easy request;
  const std::string baseUrl = "https://127.0.0.1:18080/";
};

#endif
