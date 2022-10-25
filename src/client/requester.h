#ifndef SRC_CLIENT_REQUESTER_H
#define SRC_CLIENT_REQUESTER_H

#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"


class Requester {
  public:
    std::vector<std::string> createHost(std::string username,
        std::string password);
    std::vector<std::string> loginHost(std::string username,
        std::string password);
    std::string addGameType(std::string gametype, std::string session);
    std::vector<std::string> uploadGameData(std::string session,
      std::string type, std::string host, std::string user, std::string result,
      std::string earning);
  private:
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    std::string baseUrl = "http://127.0.0.1:18080/";
};

#endif
