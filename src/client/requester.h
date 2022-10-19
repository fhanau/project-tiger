#ifndef REQUESTER_H
#define REQUESTER_H

#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"


class Requester {
  public:
    std::string createHost(std::string username, std::string password);
    std::string loginHost(std::string username, std::string password);
  private:
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    std::string baseUrl = "http://127.0.0.1:18080/";
};

#endif
