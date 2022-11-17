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
  // Sends GET request to server to create host and returns server response
  std::vector<std::string> createHost(const std::string &username,
   const std::string &password);

  // Sends GET request to server to login host and returns server response
  std::vector<std::string> loginHost(const std::string &username,
   const std::string &password);

  // Sends GET request to server to add type of game
  std::string addGameType(const std::string &gametype,
    const std::string &session);

  // Sends GET request to server to upload data and returns server response
  std::vector<std::string> uploadGameData(const std::string &session,
   const std::string &type, const std::string &host, const std::string &user,
   const std::string &result, const std::string &earning);

  // Sends GET request to server for publicly available statistics
  std::string getPublicStats(const std::string &type);

  // Sends GET request to server for total earnings across all games
  std::string getTotalEarningsAll(const std::string &session,
    const std::string &host);

  // Sends GET request to server for total earnings for a game
  std::string getTotalEarningsGame(const std::string &session,
    const std::string &host, const std::string &gametype);

  // Sends GET request to server for total earnings for a player
  std::string getTotalEarningsPlayer(const std::string &session,
    const std::string &host, const std::string &player);

  // Sends GET request to server for total wins across all games
  std::string getTotalWinsAll(const std::string &session,
    const std::string &host);

  // Sends GET request to server for total wins for a game
  std::string getTotalWinsGame(const std::string &session,
    const std::string &host, const std::string &gametype);

  // Sends GET request to server for total wins for a &
  std::string getTotalWinsPlayer(const std::string &session,
    const std::string &host, const std::string &player);

  // Sends GET request to server for total losses across all games
  std::string getTotalLossesAll(const std::string &session,
    const std::string &host);

  // Sends GET request to server for total losses for a game
  std::string getTotalLossesGame(const std::string &session,
    const std::string &host, const std::string &gametype);

  // Sends GET request to server for total losses for a player
  std::string getTotalLossesPlayer(const std::string &session,
    const std::string &host, const std::string &player);

  // Sends GET request to server for most common result for a game
  std::string getMostCommonPlay(const std::string &session,
    const std::string &host, const std::string &gametype);

  // Sends GET request to server for most common result for winners
  std::string getMostWinningPlay(const std::string &session,
    const std::string &host, const std::string &gametype);

 private:
  curlpp::Cleanup cleaner;
  curlpp::Easy request;
  std::string baseUrl = "http://127.0.0.1:18080/";
};

#endif
