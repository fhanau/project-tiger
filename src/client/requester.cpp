#include "requester.h"

std::string createHost(std::string username, std::string password) {
  std::string path = username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  return request;
}

std::string loginHost(std::string username, std::string password) {
  std::string path = username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  return request;
}
    