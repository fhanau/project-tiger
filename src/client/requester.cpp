#include "requester.h"

std::string Requester::createHost(std::string username, std::string password) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> body;
  std::string path = "create/" + username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  while (response >> tmp) {
    body.push_back(tmp);
  }
  return body[body.size() - 1];
}

std::string Requester::loginHost(std::string username, std::string password) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> body;
  std::string path = "login/" + username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  while (response >> tmp) {
    body.push_back(tmp);
  }
  return body[body.size() - 1];
}

std::string Requester::addGameType(std::string gametype, std::string session) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> body;
  std::string path = "gametype/" + gametype + "/" + session;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  while (response >> tmp) {
    body.push_back(tmp);
  }
  return body[body.size() - 1];
}

std
