#include "requester.h"

std::string Requester::createHost(std::string username, std::string password) {
  std::string path = username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  //TODO: Return request here?
  return "";
}

std::string Requester::loginHost(std::string username, std::string password) {
  std::string path = username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  //TODO: Return request here?
  return "";
}
    
