#include <sstream>
#include <iostream>
#include "requester.h"

void Requester::reset() {
  request.reset();
  //Disable SSL certificate check to allow server to use unsigned certificate.
  request.setOpt(curlpp::options::SslVerifyPeer(false));
  request.setOpt(curlpp::options::SslVerifyHost(false));
}

std::vector<std::string> Requester::createToken() {
  std::vector<std::string> payload;
  std::stringstream response;
  std::string tmp;
  std::string path = "create_account";
  std::string url = baseUrl + path;
  reset();
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  while (response >> tmp) {
    payload.push_back(tmp);
  }
  return payload;
}

std::string Requester::getTokenID(const std::string& token) {
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> payload;
  std::string path = "get_account_id";
  std::string url = baseUrl + path;
  std::string body = "token=" + token;
  request.setOpt(new curlpp::options::PostFields(body));
  request.setOpt(new curlpp::options::PostFieldSize(body.length()));
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  while (response >> tmp) {
    payload.push_back(tmp);
  }
  return payload[payload.size() - 1];
}

std::vector<std::string> Requester::uploadGameData(const std::string& token,
  const std::string& type, const std::string& user,
  const std::string& result, const std::string& earning) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> payload;
    // The type is called 'gametype' on the API side for consistency.
    std::string body = "token=" + token + "&gametype=" + type + "&player=" +
      user + "&result=" + result + "&earning=" + earning;
    std::string url = baseUrl + "upload";
    request.setOpt(new curlpp::options::PostFields(body));
    request.setOpt(new curlpp::options::PostFieldSize(body.length()));
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
  std::stringstream response;
  std::string tmp;
  std::vector<std::string> payload;
  std::string path = "public/" + type;
  std::string url = baseUrl + path;
  reset();
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  while (response >> tmp) {
    payload.push_back(tmp);
  }
  return payload[payload.size() - 1];
}

// Alex Brebenel streamline request function
std::string Requester::theRequester(const std::string &path,
    const std::string& body) {
    std::stringstream response;
    std::string tmp;
    std::vector<std::string> payload;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::PostFields(body));
    request.setOpt(new curlpp::options::PostFieldSize(body.length()));
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&response));
    request.perform();
    while (response >> tmp) {
      payload.push_back(tmp);
    }
    return payload[payload.size() - 1];
}

std::string Requester::getTotalEarningsAll(const std::string& token,
  const std::string& host) {
    std::string path = "private/total-earnings-all";
    std::string body = "token=" + token + "&hostname=" + host;
    return theRequester(path, body);
}

std::string Requester::getTotalEarningsGame(const std::string& token,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/total-earnings-game";
    std::string body = "token=" + token + "&hostname=" + host
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getTotalEarningsPlayer(const std::string& token,
  const std::string& host, const std::string& player) {
    std::string path = "private/total-earnings-player";
    std::string body = "token=" + token + "&hostname=" + host
      + "&player=" + player;
    return theRequester(path, body);
}

std::string Requester::getTotalWinsAll(const std::string& token,
  const std::string& host) {
    std::string path = "private/total-wins-all";
    std::string body = "token=" + token + "&hostname=" + host;
    return theRequester(path, body);
}

std::string Requester::getTotalWinsGame(const std::string& token,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/total-wins-game";
    std::string body = "token=" + token + "&hostname=" + host
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getTotalWinsPlayer(const std::string& token,
  const std::string& host, const std::string& player) {
    std::string path = "private/total-wins-player";
    std::string body = "token=" + token + "&hostname=" + host
      + "&player=" + player;
    return theRequester(path, body);
}

std::string Requester::getTotalLossesAll(const std::string& token,
  const std::string& host) {
    std::string path = "private/total-losses-all";
    std::string body = "token=" + token + "&hostname=" + host;
    return theRequester(path, body);
}

std::string Requester::getTotalLossesGame(const std::string& token,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/total-losses-game";
    std::string body = "token=" + token + "&hostname=" + host
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getTotalLossesPlayer(const std::string& token,
  const std::string& host, const std::string& player) {
    std::string path = "private/total-losses-player";
    std::string body = "token=" + token + "&hostname=" + host
      + "&player=" + player;
    return theRequester(path, body);
}

std::string Requester::getTotalPlayersForGame(const std::string& token,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/total-players-for-game";
    std::string body = "token=" + token + "&hostname=" + host
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getNumberOfGames(const std::string& token,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/number-of-games";
    std::string body = "token=" + token + "&hostname=" + host
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getNumberOfPlayers(const std::string& token,
  const std::string& host) {
    std::string path = "private/number-of-players";
    std::string body = "token=" + token + "&hostname=" + host;
    return theRequester(path, body);
}

// ALEX BREBENEL COMMENT - Might need to change
std::string Requester::getGreatestPlayerByWins(const std::string& token,
  const std::string& host) {
    std::string path = "private/greatest-player-by-wins";
    std::string body = "token=" + token + "&hostname=" + host;
    return theRequester(path, body);
}

std::string Requester::getMostCommonPlay(const std::string& token,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/most-common-play";
    std::string body = "token=" + token + "&hostname=" + host
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getMostWinningPlay(const std::string& token,
  const std::string& host, const std::string& gametype) {
    std::string path = "private/most-winning-play";
    std::string body = "token=" + token + "&hostname=" + host
      + "&gametype=" + gametype;
    return theRequester(path, body);
}
