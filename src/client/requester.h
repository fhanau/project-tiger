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
  std::vector<std::string> createHost(std::string username,
   std::string password);

  // Sends GET request to server to login host and returns server response
  std::vector<std::string> loginHost(std::string username,
   std::string password);

  // Sends GET request to server to add type of game
  std::string addGameType(std::string gametype, std::string session);

  // Sends GET request to server to upload data and returns server response
  std::vector<std::string> uploadGameData(std::string session,
   std::string type, std::string host, std::string user, std::string result,
   std::string earning);

  // Sends GET request to server for publicly available statistics
  std::string getPublicStats(std::string type);

  // Sends GET request to server for total earnings across all games
  std::string getTotalEarningsAll(std::string &session, 
    std::string &host);

  // Sends GET request to server for total earnings for a game
  std::string getTotalEarningsGame(std::string &session, 
    std::string &host, std::string &gametype);

  // Sends GET request to server for total earnings for a player
  std::string getTotalEarningsPlayer(std::string &session, 
    std::string &host, std::string &player);
  
  // Sends GET request to server for total wins across all games
  std::string getTotalWinsAll(std::string &session, std::string &host);

  // Sends GET request to server for total wins for a game
  std::string getTotalWinsGame(std::string &session, 
    std::string &host, std::string &gametype);

  // Sends GET request to server for total wins for a &
  std::string getTotalWinsPlayer(std::string &session, 
    std::string &host, std::string &player);

  // Sends GET request to server for total losses across all games
  std::string getTotalLossesAll(std::string &session, 
    std::string &host);

  // Sends GET request to server for total losses for a game
  std::string getTotalLossesGame(std::string &session, std::string &host,
    std::string &gametype);

  // Sends GET request to server for total losses for a player
  std::string getTotalLossesPlayer(std::string &session, 
    std::string &host, std::string &player);

  // Sends GET request to server for play that has won the most for a game
  std::string getMostWinningPlay(std::string &session, std::string &host, 
    std::string &gametype);
 private:
  curlpp::Cleanup cleaner;
  curlpp::Easy request;
  std::string baseUrl = "http://127.0.0.1:18080/";
};

#endif
