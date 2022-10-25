#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include "./parser.h"
#include "./requester.h"
#include "./util.h"

void handleCreateHost(std::vector<std::string> input,
    std::string& loggedInUsername, Requester& req, std::string& session) {
  if (input.size() != 3) {
    std::cout << "Invalid input for creating account.\n";
    std::cout << "See 'help' for a list of commands.\n";
  } else if (loggedInUsername.size() > 0) {
    std::cout << "Already logged in, please log out first.\n";
  } else {
    std::string username = input[1];
    std::string password = input[2];
    std::vector<std::string> resp = req.createHost(username, password);
    if (resp[0].compare("ERROR")) {
      loggedInUsername = username;
      session = resp[1];
      std::cout << "Successfully logged into " << loggedInUsername << ".\n";
    } else {
      std::cout << "Unsuccessful account creation.\n";
      std::cout << "ERROR: " << resp[1] << "\n";
    }
  }
}

void handleLoginHost(std::vector<std::string> input,
    std::string& loggedInUsername, Requester& req, std::string& session) {
  if (input.size() != 3) {
    std::cout << "Invalid input for logging into account.\n";
    std::cout << "See 'help' for a list of commands.\n";
  } else if (loggedInUsername.size() > 0) {
    std::cout << "Already logged in. Please log out first.\n";
  } else {
    std::string username = input[1];
    std::string password = input[2];
    std::vector<std::string> resp = req.loginHost(username, password);
    if (resp[0].compare("ERROR")) {
      loggedInUsername = username;
      session = resp[1];
      std::cout << "Successfully logged into " << loggedInUsername << ".\n";
    } else {
      std::cout << "Unsuccessful login.\n";
      std::cout << "ERROR: " << resp[1] << "\n";
    }
  }
}

void handleAddGameType(std::vector<std::string> input,
    std::string& loggedInUsername, Requester& req, std::string& session) {
  if (input.size() != 2) {
    std::cout << "Invalid input for uploading type of game.\n";
    std::cout << "See 'help' for a list of commands.\n";
  } else if (loggedInUsername.size() == 0) {
    std::cout << "Must be logged in to add game type." << " ";
    std::cout << "See 'help' for a list of commands.\n";
  } else {
    std::string gametype = input[1];
    std::string resp = req.addGameType(gametype, session);
    if (resp.size() > 0) {
      std::cout << "Successfully added gametype to server.\n";
    } else {
      std::cout << "Could not add gametype to server.\n";
    }
  }
}

void handleUploadGameData(std::vector<std::string> input,
    std::string& loggedInUsername, Requester& req, std::string& session) {
  if (input.size() < 5) {
    std::cout << "Invalid input for uploading game data.\n";
    std::cout << "See 'help' for a list of commands.\n";
  } else if (loggedInUsername.size() == 0) {
    std::cout << "Must be logged in to upload game data." << " ";
    std::cout << "See 'help' for a list of commands.\n";
  } else {
    std::string type = input[1];
    std::string user = input[2];
    /*int*/std::string earning = std::stoi(input[3]);
    std::string result = formatResult(input);
    std::vector<std::string> resp = req.uploadGameData(session, type,
      loggedInUsername, user, result, earning);
    if (resp[0].compare("ERROR")) {
      std::cout << "Successfully uploaded game data.\n";
    } else {
      std::cout << "Unsuccessful game data upload\n";
      std::cout << "ERROR: " << resp[1] << "\n";
    }
  }
}

std::string formatResult(std::vector<std::string> input) {
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

void displayHelp() {
  std::cout << "Commands available:\n";
  std::cout << "create <string: username> <string: password>\n";
  std::cout << "login <string: username> <string: password>\n";
  std::cout << "gametype <string: name of game>\n";
  std::cout << "upload <string: name of game> <string: winning userid> <int: money earned> <string: result description>";
  std::cout << "logout\n";
  std::cout << "exit\n";
}

void handleLogoutHost(std::string& loggedInUsername, std::string& session) {
  std::cout << "Successfully logged out of " << loggedInUsername << ".\n";
  loggedInUsername = "";
  session = "";
}

void handleExit() {
  std::cout << "Exiting client.\n";
  exit(0);
}

void processCleanInput(std::vector<std::string>& cleanInput,
    std::string& loggedInUsername, Requester& req, std::string &session) {
  std::string command = cleanInput[0];
  if (!command.compare("create")) {
    handleCreateHost(cleanInput, loggedInUsername, req, session);
  } else if (!command.compare("login")) {
    handleLoginHost(cleanInput, loggedInUsername, req, session);
  } else if (!command.compare("gametype")) {
    handleAddGameType(cleanInput, loggedInUsername, req, session);
  } else if (!command.compare("upload")) {
    handleUploadGameData(cleanInput, loggedInUsername, req, session);
  } else if (!command.compare("help")) {
    displayHelp();
  } else if (!command.compare("logout")) {
    handleLogoutHost(loggedInUsername, session);
  } else if (!command.compare("exit")) {
    handleExit();
  }
}
