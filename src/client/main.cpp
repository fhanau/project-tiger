#include <iostream>
#include <memory>
#include <string>

#include "parser.h"
#include "requester.h"

void handleCreateHost(std::vector<std::string> input, std::string& loggedInUsername) {
  if (input.size() != 3) {
    cout << "Invalid input for creating account.\n";
  } else {
    std::string username = input[1];
    std::string password = input[2];
    // This is where request to CROW(/create) is made
    // if validated by server
    loggedInUsername = username;
    cout << "Successfully logged into " << loggedInUsername << ".\n";
  }
};

void handleLoginHost(std::vector<std::string> input, std::string& loggedInUsername) {
  if (input.size() != 3) {
    cout << "Invalid input for logging into account.\n";
  } else {
    std::string username = input[1];
    std::string password = input[2];
    // This is where request to CROW(/login) is made
    // if validated by server
    loggedInUsername = username;
    cout << "Successfully logged into " << loggedInUsername << ".\n";
  }
};

void displayHelp() {
  cout << "Commands available:\n";
  cout << "create <string: username> <string: password>\n";
  cout << "login <string: username> <string: password>\n";
  cout << "logout\n";
  cout << "exit\n";
};

void handleLogoutHost(std::string& loggedInUsername) {
  cout << "Successfully logged out of " << loggedInUsername << ".\n";
  loggedInUsername = "";
};

void handleExit() {
  cout << "Exiting client.\n";
  exit(0);
}

void processCleanInput(std::vector<std::string>& cleanInput, std::string& loggedInUsername) {
  std::string command = cleanInput[0];
  if (!command.compare("create")) {
    handleCreateHost(cleanInput, loggedInUsername);
  } else if (!command.compare("login")) {
    handleLoginHost(cleanInput, loggedInUsername);
  } else if (!command.compare("help")) {
    displayHelp();
  } else if (!command.compare("logout")) {
    handleLogoutHost(loggedInUsername);
  } else if (!command.compare("exit")) {
    handleExit();
  }
};

int main(int argc, char** argv) {
  std::string userInput;
  std::vector<std::string> cleanInput;
  Parser clientParser;
  std::string loggedInUsername;
  cout << "Welcome to Project Tiger!\n";
  cout << "A list of commands can be displayed by typing 'help' and hitting enter.\n";
  cout << "Otherwise you can begin entering commands.\n";
  while (true) {
    cout << ">" << " ";
    getline(cin, userInput);
    int validUserInput = clientParser.verifyInput(userInput);
    if (validUserInput) {
      cleanInput = clientParser.getCleanInput();
      processCleanInput(cleanInput, loggedInUsername);
    } else {
      cout << "Invalid command. Type 'help' and hit enter to see a list of valid commands.\n";
    }
  }
}
