#include <iostream>
#include <memory>
#include <string>

#include "helper/parser.h"

using namespace std;

string loggedInUsername = "";

void handleCreateHost(vector<string> input) {
  if (input.size() != 3) {
    cout << "Invalid input for creating account.";
  } else {
    string username = input.at(1);
    string password = input.at(2);
    // This is where request to CROW(/create) is made
    // if validated by server
    loggedInUsername = username;
    cout << "Successfully logged into " << loggedInUsername << ".\n";
  }
};

void handleLoginHost(vector<string> input) {
  if (input.size() != 3) {
    cout << "Invalid input for logging into account.";
  } else {
    string username = input.at(1);
    string password = input.at(2);
    // This is where request to CROW(/login) is made
    // if validated by server
    loggedInUsername = username;
    cout << "Successfully logged into " << loggedInUsername << ".\n";
  }
};

void displayHelp() {
  cout << "Commands available:";
  cout << "create <string: username> <string: password>";
  cout << "login <string: username> <string: password>";
  cout << "logout";
  cout << "exit";
};

void handleLogoutHost() {
  cout << "Successfully logged out of " << loggedInUsername << ".\n";
  loggedInUsername = "";
};

void handleExit() {
  cout << "Exiting client.";
  exit(0);
}

void processCleanInput(vector<string> cleanInput) {
  string command = cleanInput.at(0);
  if (!command.compare("create")) {
    handleCreateHost(cleanInput);
  } else if (!command.compare("login")) {
    handleLoginHost(cleanInput);
  } else if (!command.compare("help")) {
    displayHelp();
  } else if (!command.compare("logout")) {
    handleLogoutHost();
  } else if (!command.compare("exit")) {
    handleExit();
  }
};

int main(int argc, char** argv) {
  string userInput;
  vector<string> cleanInput;
  Parser clientParser;
  cout << "Welcome to Project Tiger!";
  cout << "A list of commands can be displayed by typing 'help' and hitting enter.";
  cout << "Otherwise you can begin entering commands.";
  while (true) {
    cout << ">" << " ";
    cin >> userInput;
    int validUserInput = clientParser.verifyInput(userInput);
    if (validUserInput) {
      cleanInput = clientParser.getCleanInput();
      processCleanInput(cleanInput);
    } else {
      cout << "Invalid command. Type 'help' and hit enter to see a list of valid commands.";
    }
  }
}