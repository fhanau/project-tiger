#include "requester.h"

std::vector<std::string> Requester::createHost(std::string username, std::string password) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> payload;
  std::string path = "create/" + username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  int isFirstBodyString = 0;
  while(response >> tmp) {
    if (!isFirstBodyString) {
      isFirstBodyString = 1;
      if (tmp.compare("ERROR") && tmp.compare("SUCCESS")) {
        payload.push_back("ERROR");
        payload.push_back("Error connecting to server.");
        return payload;
      }
    }

    if (payload.size() > 0) {
      payload.push_back(tmp);
      return payload;
    }

    if (!tmp.compare("ERROR") || !tmp.compare("SUCCESS")) {
      payload.push_back(tmp);
    }
  }
  payload.push_back("ERROR");
  payload.push_back("No data from server.");
  return payload;
}

std::vector<std::string> Requester::loginHost(std::string username, std::string password) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> payload;
  std::string path = "login/" + username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  int isFirstBodyString = 0;
  while (response >> tmp) {
    if (!isFirstBodyString) {
      isFirstBodyString = 1;
      if (tmp.compare("ERROR") && tmp.compare("SUCCESS")) {
        payload.push_back("ERROR");
        payload.push_back("Error connecting to server.");
        return payload;
      }
    }

    if (payload.size() > 0) {
      payload.push_back(tmp);
      return payload;
    }

    if (!tmp.compare("ERROR") || !tmp.compare("SUCCESS")) {
      payload.push_back(tmp);
    }
  }
  payload.push_back("ERROR");
  payload.push_back("No data from server.");
  return payload;
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
  while(response >> tmp) {
    body.push_back(tmp);
  }
  return body[body.size() - 1];
}
