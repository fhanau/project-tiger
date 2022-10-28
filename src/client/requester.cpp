#include "requester.h"

std::vector<std::string> Requester::createHost(std::string username,
    std::string password) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> payload;
  std::string path = "create/" + username + "/" + password;
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

std::vector<std::string> Requester::loginHost(std::string username,
    std::string password) {
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
  while (response >> tmp) {
    body.push_back(tmp);
  }
  return body[body.size() - 1];
}

std::vector<std::string> Requester::uploadGameData(std::string session,
  std::string type, std::string host, std::string user,
  std::string result, std::string earning) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> payload;
    std::string firstPath = "upload/" + session + "/" + type + "/" + host;
    std::string secondPath = "/" + user + "/" + result + "/" + earning;
    std::string path = firstPath + secondPath;
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

std::string Requester::getPublicStats(std::string type) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> body;
  std::string path = "public/" + type;
  std::string url = baseUrl + path;
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  while (response >> tmp) {
    body.push_back(tmp);
  }
  return body[body.size() - 1];
}

std::string Requester::getTotalEarningsAll(std::string &session, 
  std::string &host) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/total-earnings-all/";
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}

std::string Requester::getTotalEarningsGame(std::string &session, 
  std::string &host, std::string &gametype) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/total-earnings-game/" + host + "/" + gametype;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}

std::string Requester::getTotalEarningsPlayer(std::string &session, 
  std::string &host, std::string &player) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/total-earnings-player/" + host + "/" + player;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}
  
std::string Requester::getTotalWinsAll(std::string &session, 
  std::string &host) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/total-wins-all";
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}

std::string Requester::getTotalWinsGame(std::string &session, 
  std::string &host, std::string &gametype) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/total-wins-game/" + host + "/" + gametype;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}

std::string Requester::getTotalWinsPlayer(std::string &session, 
  std::string &host, std::string &player) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/total-wins-player/" + host + "/" + player;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}

std::string Requester::getTotalLossesAll(std::string &session, 
  std::string &host) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/total-losses-all";
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}

std::string Requester::getTotalLossesGame(std::string &session, 
  std::string &host, std::string &gametype) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/total-losses-game/" + host + "/" + gametype;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}

std::string Requester::getTotalLossesPlayer(std::string &session, 
  std::string &host, std::string &player) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/total-losses-player/" + host + "/" + player;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}

std::string Requester::getMostWinningPlay(std::string &session, 
  std::string &host, std::string &gametype) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path = "private/most-winning-play/" + host + "/" + gametype;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}
