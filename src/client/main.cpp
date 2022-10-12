#include <iostream>
#include <memory>
#include <string>

#include "helper/parser.h"

using namespace std;

void handleCreateHost(vector<string> input, string& loggedInUsername) {
  if (input.size() != 3) {
    cout << "Invalid input for creating account.";
  } else {
    string username = input[1];
    string password = input[2];
    // This is where request to CROW(/create) is made
    // if validated by server
    loggedInUsername = username;
    cout << "Successfully logged into " << loggedInUsername << ".\n";
  }
};

void handleLoginHost(vector<string> input, string& loggedInUsername) {
  if (input.size() != 3) {
    cout << "Invalid input for logging into account.";
  } else {
    string username = input[1];
    string password = input[2];
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

void handleLogoutHost(string& loggedInUsername) {
  cout << "Successfully logged out of " << loggedInUsername << ".\n";
  loggedInUsername = "";
};

void handleExit() {
  cout << "Exiting client.";
  exit(0);
}

void processCleanInput(vector<string>& cleanInput, string& loggedInUsername) {
  string command = cleanInput[0];
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
  string userInput;
  vector<string> cleanInput;
  Parser clientParser;
  string loggedInUsername;
  cout << "Welcome to Project Tiger!";
  cout << "A list of commands can be displayed by typing 'help' and hitting enter.";
  cout << "Otherwise you can begin entering commands.";
  while (true) {
    cout << ">" << " ";
    cin >> userInput;
    int validUserInput = clientParser.verifyInput(userInput);
    if (validUserInput) {
      cleanInput = clientParser.getCleanInput();
      processCleanInput(cleanInput, loggedInUsername);
    } else {
      cout << "Invalid command. Type 'help' and hit enter to see a list of valid commands.";
    }
  }
}