#include "requester.h"

std::vector<std::string> Requester::createHost(std::string username, std::string password) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> body;
  std::vector<std::string> payload;
  std::string path = "create/" + username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  int foundHttpCode = 0;
  while(response >> tmp) {
    if (!foundHttpCode) {
      foundHttpCode = 1;
      if (tmp.compare("200")) {
        payload.push_back("ERROR");
        payload.push_back("Unable to connect to server");
        return payload;
      }
    }

    if (payload.size() > 0) {
      payload.push_back(tmp);
      return payload;
    }

    if (tmp.compare("ERROR")) {
      payload.push_back("ERROR");
    }
    body.push_back(tmp);
  }
  payload.push_back("SUCCESS");
  payload.push_back(body[body.size() - 1]);
  return payload;
}

std::vector<std::string> Requester::loginHost(std::string username, std::string password) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> body;
  std::vector<std::string> payload;
  std::string path = "login/" + username + "/" + password;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  int foundHttpCode = 0;
  while(response >> tmp) {
    if (!foundHttpCode) {
      foundHttpCode = 1;
      if (tmp.compare("200")) {
        payload.push_back("ERROR");
        payload.push_back("Unable to connect to server");
        return payload;
      }
    }

    if (payload.size() > 0) {
      payload.push_back(tmp);
      return payload;
    }

    if (tmp.compare("ERROR")) {
      payload.push_back("ERROR");
    }
    body.push_back(tmp);
  }
  payload.push_back("SUCCESS");
  payload.push_back(body[body.size() - 1]);
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
