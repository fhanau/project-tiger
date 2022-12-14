#include <fstream>
#include <iostream>
#include <string>
#include "requester.h"
#include "util.h"

int create_token(Requester& req, std::string& token) {
  std::vector<std::string> resp = req.createToken();
  if (resp[0].compare("ERROR")) {
    token = resp[0];
    std::cout << "Successfully created token.\n";
    return EXIT_SUCCESS;
  }
  std::cout << "Unsuccessful token creation.\n";
  std::cout << "ERROR: " << resp[1] << "\n";
  return EXIT_FAILURE;
}

int load_token(std::string& token, const std::string& path) {
  std::ifstream token_file(path);
  if (!token_file.good()) {
    return EXIT_FAILURE;
  }
  std::stringstream buf;
  buf << token_file.rdbuf();
  buf >> token;
  return EXIT_SUCCESS;
}

void save_token(const std::string& token, const std::string& path) {
  std::ofstream token_file(path);
  token_file << token;
}

std::string get_id(Requester& req, const std::string& token) {
  return req.getTokenID(token);
}

std::string formatResult(const std::vector<std::string> &input) {
  int pointer = 4;
  std::string result = "";
  while (pointer < input.size()) {
    result += input[pointer];
    if (pointer != input.size() - 1) {
      result += " ";
    }
    pointer++;
  }
  return result;
}

void handleUploadGameData(const std::vector<std::string> &input, Requester& req,
    const std::string &session) {
  if (input.size() < 5) {
    std::cout << "Invalid input for uploading game data.\n";
    std::cout << "See 'help' for a list of commands.\n";
  } else {
    std::string type = input[1];
    std::string user = input[2];
    if (type.size() > 50 || user.size() > 50) {
      std::cout << "Game name and player name must be under 50 characters.\n";
    }
    std::string earning = input[3];
    std::string result = formatResult(input);
    std::vector<std::string> resp = req.uploadGameData(session, type,
      user, result, earning);
    if (resp[0].compare("ERROR")) {
      std::cout << "Successfully uploaded game data.\n";
    } else {
      std::cout << "Unsuccessful game data upload.\n";
      std::cout << "ERROR: " << resp[1] << "\n";
    }
  }
}

void handlePublicStats(const std::vector<std::string> &input, Requester& req) {
  if (input.size() != 2) {
    std::cout << "Invalid input for requesting public data.\n";
    std::cout << "See 'help' for a list of commands.\n";
  } else {
    std::string type = input[1];
    std::string resp = req.getPublicStats(type);
    if (resp.size() > 0) {
      std::cout << type << ": " << resp << "\n";
    } else {
      std::cout << "Could not retrieve public data.\n";
    }
  }
}

void handlePrivateStats(const std::vector<std::string> &input, Requester& req,
  const std::string &session) {
    if (input.size() < 2) {
      std::cout << "Invalid input for requesting private data.\n";
      std::cout << "See 'help' for a list of commands.\n";
    } else if (session.size() == 0) {
      std::cout << "Cannot request private data without being logged in.\n";
      std::cout << "See 'help' for a list of commands.\n";
    } else {
      std::string type = input[1];
      if (!type.compare("total-earnings-all")) {
        std::string resp = req.getTotalEarningsAll(session);
        if (resp.size() > 0) {
          std::cout << type << ": " << resp << "\n";
        } else {
          std::cout << "Could not retrieve private data.\n";
        }
      } else if (!type.compare("total-earnings-game")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for total-earnings-game.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string resp = req.getTotalEarningsGame(session,
            gametype);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("total-earnings-player")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for total-earnings-player.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string playerid = input[2];
          std::string resp = req.getTotalEarningsPlayer(session,
            playerid);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("total-wins-all")) {
        std::string resp = req.getTotalWinsAll(session);
        if (resp.size() > 0) {
          std::cout << type << ": " << resp << "\n";
        } else {
          std::cout << "Could not retrieve private data.\n";
        }
      } else if (!type.compare("total-wins-game")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for total-wins-game.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string resp = req.getTotalWinsGame(session,
            gametype);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("total-wins-player")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for total-wins-player.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string playerid = input[2];
          std::string resp = req.getTotalWinsPlayer(session,
            playerid);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("total-losses-all")) {
        std::string resp = req.getTotalLossesAll(session);
        if (resp.size() > 0) {
          std::cout << type << ": " << resp << "\n";
        } else {
          std::cout << "Could not retrieve private data.\n";
        }
      } else if (!type.compare("total-losses-game")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for total-losses-game.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string resp = req.getTotalLossesGame(session,
            gametype);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("total-losses-player")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for total-earnings-player.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string playerid = input[2];
          std::string resp = req.getTotalLossesPlayer(session,
            playerid);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("most-common-play")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for most-common-play.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string resp = req.getMostCommonPlay(session,
            gametype);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("most-winning-play")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for most-winning-play.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string resp = req.getMostWinningPlay(session,
            gametype);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("total-players-for-game")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for total-players-for-game.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string resp = req.getTotalPlayersForGame(session,
            gametype);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("greatest-player-by-wins")) {
        std::string resp = req.getGreatestPlayerByWins(session);
        if (resp.size() > 0) {
          std::cout << type << ": " << resp << "\n";
        } else {
          std::cout << "Could not retrieve private data.\n";
        }
      } else if (!type.compare("number-of-results")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for number-of-games.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string resp = req.getNumberOfResults(session,
            gametype);
          if (resp.size() > 0) {
            std::cout << "number-of-results: " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("number-of-players")) {
        std::string resp = req.getNumberOfPlayers(session);
        if (resp.size() > 0) {
          std::cout << type << ": " << resp << "\n";
        } else {
          std::cout << "Could not retrieve private data.\n";
        }
      } else if (!type.compare("total-player-earning-for-game")) {
        if (input.size() != 4) {
          std::cout << "Invalid parameters for "
            "total-player-earning-for-game.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string playerid = input[3];
          std::string resp = req.getTotalPlayerEarningsForGame(session,
            gametype, playerid);
          if (resp.size() > 0) {
            std::cout << "total-player-earning-for-game: " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("total-player-debt-for-game")) {
        if (input.size() != 4) {
          std::cout << "Invalid parameters for "
           "total-player-debt-for-game.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string playerid = input[3];
          std::string resp = req.getTotalPlayerDebtForGame(session,
            gametype, playerid);
          if (resp.size() > 0) {
            std::cout << "total-player-debt-for-game: " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("total-player-wins-for-game")) {
        if (input.size() != 4) {
          std::cout << "Invalid parameters for total-player-wins-for-game.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string playerid = input[3];
          std::string resp = req.getTotalPlayerWinsForGame(session,
            gametype, playerid);
          if (resp.size() > 0) {
            std::cout << "total-player-wins-for-game: " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("total-player-losses-for-game")) {
        if (input.size() != 4) {
          std::cout << "Invalid parameters for"
            " total-player-losses-for-game.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string playerid = input[3];
          std::string resp = req.getTotalPlayerLossesForGame(session,
            gametype, playerid);
          if (resp.size() > 0) {
            std::cout << "total-player-losses-for-game: " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("most-winning-player-for-game")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for "
            "most-winning-player-for-game.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string resp = req.getMostWinningPlayerForGame(session,
            gametype);
          if (resp.size() > 0) {
            std::cout << "most-winning-player-for-game: " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else if (!type.compare("median-earning")) {
        if (input.size() != 3) {
          std::cout << "Invalid parameters for median-earning.\n";
          std::cout << "See 'help-private' for a list of commands.\n";
        } else {
          std::string gametype = input[2];
          std::string resp = req.getMedianEarning(session,
            gametype);
          if (resp.size() > 0) {
            std::cout << type << ": " << resp << "\n";
          } else {
            std::cout << "Could not retrieve private data.\n";
          }
        }
      } else {
        std::cout << "Invalid type of private request.\n";
        std::cout << "See 'help-private' for a list of commands.\n";
      }
    }
}

// Examples of private commands. They match url path
void displayPrivateHelp() {
  std::cout << "Commands available for private data requests:\n";
  std::cout << "private total-earnings-all\n";
  std::cout << "private total-earnings-game <string: game name>\n";
  std::cout << "private total-earnings-player <string: playerid>\n";
  std::cout << "private total-wins-all\n";
  std::cout << "private total-wins-game <string: game name>\n";
  std::cout << "private total-wins-player <string: playerid>\n";
  std::cout << "private total-losses-all\n";
  std::cout << "private total-losses-game <string: game name>\n";
  std::cout << "private total-losses-player <string: playerid>\n";
  std::cout << "private most-common-play <string: game name>\n";
  std::cout << "private most-winning-play <string: game name>\n";
  std::cout << "private number-of-results <string: game name>\n";
  std::cout << "private total-players-for-game <string: game name>\n";
  std::cout << "private greatest-player-by-wins\n";
  std::cout << "private total-player-earning-for-game <string: game name>"
   " <string: playerid>\n";
  std::cout << "private total-player-debt-for-game <string: game name>"
   " <string: playerid>\n";
  std::cout << "private total-player-wins-for-game <string: game name>"
    " <string: playerid>\n";
  std::cout << "private total-player-losses-for-game <string: game name>"
    " <string: playerid>\n";
  std::cout << "private most-winning-player-for-game <string: game name>\n";
  std::cout << "private number-of-players\n";
  std::cout << "private median-earning <string: game name>\n";
}

void displayHelp() {
  std::cout << "Commands available:\n";
  std::cout << "upload <string: name of game> <string: playerid>" << " ";
  std::cout << "<int: win/loss amount> <string: result description>\n";
  std::cout << "public <string: ['total-games', 'total-players'," << " ";
  std::cout << "'total-types']>\n";
  std::cout << "help-private\n";
  std::cout << "exit\n";
}

void handleExit() {
  std::cout << "Exiting client.\n";
  exit(0);
}

void processCleanInput(const std::vector<std::string>& cleanInput,
    Requester& req, const std::string &token) {
  std::string command = cleanInput[0];
  if (!command.compare("upload")) {
    handleUploadGameData(cleanInput, req, token);
  } else if (!command.compare("public")) {
    handlePublicStats(cleanInput, req);
  } else if (!command.compare("private")) {
    handlePrivateStats(cleanInput, req, token);
  } else if (!command.compare("help")) {
    displayHelp();
  } else if (!command.compare("help-private")) {
    displayPrivateHelp();
  } else if (!command.compare("exit")) {
    handleExit();
  }
}
