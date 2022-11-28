#include <sstream>
#include <iostream>
#include "requester.h"

void Requester::reset() {
  request.reset();
  // Disable SSL certificate check to allow server to use unsigned certificate.
  request.setOpt(curlpp::options::SslVerifyPeer(false));
  request.setOpt(curlpp::options::SslVerifyHost(false));
}

void Requester::perform(std::vector<std::string>& payload,
    const std::string& url) {
  std::stringstream response;
  std::string tmp;
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::WriteStream(&response));
  request.perform();
  while (response >> tmp) {
    payload.push_back(tmp);
  }
}

std::vector<std::string> Requester::createToken() {
  std::vector<std::string> payload;
  std::string path = "create_account";
  std::string url = baseUrl + path;
  reset();
  perform(payload, url);
  return payload;
}

std::string Requester::getTokenID(const std::string& token) {
  std::string path = "get_account_id";
  std::string body = "token=" + token;
  return theRequester(path, body);
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
  std::vector<std::string> payload;
  std::string path = "public/" + type;
  std::string url = baseUrl + path;
  reset();
  perform(payload, url);
  return payload[payload.size() - 1];
}

// Alex Brebenel streamline request function
std::string Requester::theRequester(const std::string &path,
    const std::string& body) {
    std::vector<std::string> payload;
    std::string url = baseUrl + path;
    request.setOpt(new curlpp::options::PostFields(body));
    request.setOpt(new curlpp::options::PostFieldSize(body.length()));
    perform(payload, url);
    return payload[payload.size() - 1];
}

std::string Requester::getTotalEarningsAll(const std::string& token) {
    std::string path = "private/total-earnings-all";
    std::string body = "token=" + token;
    return theRequester(path, body);
}

std::string Requester::getTotalEarningsGame(const std::string& token,
  const std::string& gametype) {
    std::string path = "private/total-earnings-game";
    std::string body = "token=" + token
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getTotalEarningsPlayer(const std::string& token,
  const std::string& player) {
    std::string path = "private/total-earnings-player";
    std::string body = "token=" + token
      + "&player=" + player;
    return theRequester(path, body);
}

std::string Requester::getTotalWinsAll(const std::string& token) {
    std::string path = "private/total-wins-all";
    std::string body = "token=" + token;
    return theRequester(path, body);
}

std::string Requester::getTotalWinsGame(const std::string& token,
  const std::string& gametype) {
    std::string path = "private/total-wins-game";
    std::string body = "token=" + token
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getTotalWinsPlayer(const std::string& token,
  const std::string& player) {
    std::string path = "private/total-wins-player";
    std::string body = "token=" + token
      + "&player=" + player;
    return theRequester(path, body);
}

std::string Requester::getTotalLossesAll(const std::string& token) {
    std::string path = "private/total-losses-all";
    std::string body = "token=" + token;
    return theRequester(path, body);
}

std::string Requester::getTotalLossesGame(const std::string& token,
  const std::string& gametype) {
    std::string path = "private/total-losses-game";
    std::string body = "token=" + token
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getTotalLossesPlayer(const std::string& token,
  const std::string& player) {
    std::string path = "private/total-losses-player";
    std::string body = "token=" + token
      + "&player=" + player;
    return theRequester(path, body);
}

std::string Requester::getTotalPlayersForGame(const std::string& token,
  const std::string& gametype) {
    std::string path = "private/total-players-for-game";
    std::string body = "token=" + token
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getNumberOfResults(const std::string& token,
  const std::string& gametype) {
    std::string path = "private/number-of-games";
    std::string body = "token=" + token
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getNumberOfPlayers(const std::string& token) {
    std::string path = "private/number-of-players";
    std::string body = "token=" + token;
    return theRequester(path, body);
}

// ALEX BREBENEL COMMENT - Might need to change
std::string Requester::getGreatestPlayerByWins(const std::string& token) {
    std::string path = "private/greatest-player-by-wins";
    std::string body = "token=" + token;
    return theRequester(path, body);
}

std::string Requester::getMostCommonPlay(const std::string& token,
  const std::string& gametype) {
    std::string path = "private/most-common-play";
    std::string body = "token=" + token
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

std::string Requester::getMostWinningPlay(const std::string& token,
  const std::string& gametype) {
    std::string path = "private/most-winning-play";
    std::string body = "token=" + token
      + "&gametype=" + gametype;
    return theRequester(path, body);
}

// Sends POST request to server for total earnings for a player in a specific game
std::string Requester::getTotalPlayerEarningsForGame(const std::string& token,
  const std::string& gametype, const std::string& player) {
    std::string path = "private/total-player-earning-for-game";
    std::string body = "token=" + token
      + "&gametype=" + gametype + "&player=" + player;
    return theRequester(path, body);
}

// Sends POST request to server for total debt for a player in a specific game
std::string Requester::getTotalPlayerDebtForGame(const std::string& token,
  const std::string& gametype, const std::string& player) {
    std::string path = "private/total-player-debt-for-game";
    std::string body = "token=" + token
      + "&gametype=" + gametype + "&player=" + player;
    return theRequester(path, body);
}

// Sends POST request to server for total wins for a player in a specific game
std::string Requester::getTotalPlayerWinsForGame(const std::string& token,
  const std::string& gametype, const std::string& player) {
    std::string path = "private/total-player-wins-for-game";
    std::string body = "token=" + token
      + "&gametype=" + gametype + "&player=" + player;
    return theRequester(path, body);
}

// Sends POST request to server for total lossing for a player in a specific game
std::string Requester::getTotalPlayerLossesForGame(const std::string& token,
  const std::string& gametype, const std::string& player) {
    std::string path = "private/total-player-losses-for-game";
    std::string body = "token=" + token
      + "&gametype=" + gametype + "&player=" + player;
    return theRequester(path, body);
}

// Sends POST request to server for most winning player for a specific game
std::string Requester::getMostWinningPlayerForGame(const std::string& token,
  const std::string& gametype) {
    std::string path = "private/most-winning-player-for-game";
    std::string body = "token=" + token
      + "&gametype=" + gametype;
    return theRequester(path, body);
}
