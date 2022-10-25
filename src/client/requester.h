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
 private:
  curlpp::Cleanup cleaner;
  curlpp::Easy request;
  std::string baseUrl = "http://127.0.0.1:18080/";
};

#endif
