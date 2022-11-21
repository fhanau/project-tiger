#include <iostream>
#include "requester.h"

std::vector<std::string> Requester::createHost(const std::string& username,
    const std::string& password) {
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

std::vector<std::string> Requester::loginHost(const std::string& username,
    const std::string& password) {
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

std::vector<std::string> Requester::uploadGameData(const std::string& session,
  const std::string& type, const std::string& host, const std::string& user,
  const std::string& result, const std::string& earning) {
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

std::string Requester::getPublicStats(const std::string& type) {
  std::string path = "public/" + type;
  return theRequester(path);
}

// Alex Brebenel streamline request function
std::string Requester::theRequester(const std::string &path) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> body;
    std::string path;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      body.push_back(tmp);
    }
    return body[body.size() - 1];
}

std::string Requester::getTotalEarningsAll(const std::string& session,
  const std::string& host) {
    std::string path = "private/total-earnings-all/" + session + "/" + host;
    return theRequester(path);
}

std::string Requester::getTotalEarningsGame(const std::string& session,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/total-earnings-game/" + session + "/" + host
      + "/" + gametype;
    return theRequester(path);
}

std::string Requester::getTotalEarningsPlayer(const std::string& session,
  const std::string& host, const std::string& player) {
    std::string path = "private/total-earnings-player/" + session + "/" + host
      + "/" + player;
    return theRequester(path);
}

std::string Requester::getTotalWinsAll(const std::string& session,
  const std::string& host) {
    std::string path = "private/total-wins-all/" + session + "/" + host;
    return theRequester(path);
}

std::string Requester::getTotalWinsGame(const std::string& session,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/total-wins-game/" + session + "/" + host
      + "/" + gametype;
    return theRequester(path);
}

std::string Requester::getTotalWinsPlayer(const std::string& session,
  const std::string& host, const std::string& player) {
    std::string path = "private/total-wins-player/" + session + "/" + host
      + "/" + player;
    return theRequester(path);
}

std::string Requester::getTotalLossesAll(const std::string& session,
  const std::string& host) {
    std::string path = "private/total-losses-all/" + session + "/" + host;
    return theRequester(path);
}

std::string Requester::getTotalLossesGame(const std::string& session,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/total-losses-game/" + session + "/" + host
      + "/" + gametype;
    return theRequester(path);
}

std::string Requester::getTotalLossesPlayer(const std::string& session,
  const std::string& host, const std::string& player) {
    std::string path = "private/total-losses-player/" + session + "/" + host
      + "/" + player;
    return theRequester(path);
}

std::string Requester::getTotalPlayersForGame(const std::string& session,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/total-players-for-game/" + session + "/" + host
      + "/" + gametype;
    return theRequester(path);
}

std::string Requester::getNumberOfGames(const std::string& session,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/number-of-games/" + session + "/" + host
      + "/" + gametype;
    return theRequester(path);
}

std::string Requester::getNumberOfPlayers(const std::string& session,
  const std::string& host) {
    std::string path = "private/number-of-players/" + session + "/" + host;
    return theRequester(path);
}

// ALEX BREBENEL COMMENT - Might need to change
std::string Requester::getGreatestPlayerByWins(const std::string& session,
  const std::string& host) {
    std::string path = "private/greatest-player-by-wins/" + session + "/" +
      host;
    return theRequester(path);
}

std::string Requester::getMostCommonPlay(const std::string& session,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/most-common-play/" + session + "/" + host
      + "/" + gametype;
    return theRequester(path);
}

std::string Requester::getMostWinningPlay(const std::string& session,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/most-winning-play/" + session + "/" + host
      + "/" + gametype;
    return theRequester(path);
}
