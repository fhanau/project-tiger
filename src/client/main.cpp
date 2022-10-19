#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

#include "parser.h"
#include "requester.h"

void handleCreateHost(std::vector<std::string> input, std::string& loggedInUsername,
Requester& req, std::string& session) {
  if (input.size() != 3) {
    std::cout << "Invalid input for creating account.\n";
  } else {
    std::string username = input[1];
    std::string password = input[2];
    std::string resp = req.createHost(username, password);
    if (resp.size() > 0) {
      loggedInUsername = username;
      session = resp;
      std::cout << "Successfully logged into " << loggedInUsername << ".\n";
    } else {
      std::cout << "Unsuccessful account creation please try again.\n";
    }   
  }
};

void handleLoginHost(std::vector<std::string> input, std::string& loggedInUsername, 
Requester& req, std::string& session) {
  if (input.size() != 3) {
    std::cout << "Invalid input for logging into account.\n";
  } else {
    std::string username = input[1];
    std::string password = input[2];
    std::string resp = req.loginHost(username, password);
    if (resp.size() > 0) {
      loggedInUsername = username;
      session = resp;
      std::cout << "Successfully logged into " << loggedInUsername << ".\n";
    } else {
      std::cout << "Unsuccessful login please try again.\n";
    }  
  }
};

void displayHelp() {
  std::cout << "Commands available:\n";
  std::cout << "create <string: username> <string: password>\n";
  std::cout << "login <string: username> <string: password>\n";
  std::cout << "logout\n";
  std::cout << "exit\n";
};

void handleLogoutHost(std::string& loggedInUsername, std::string& session) {
  std::cout << "Successfully logged out of " << loggedInUsername << ".\n";
  loggedInUsername = "";
  session = "";
};

void handleExit() {
  std::cout << "Exiting client.\n";
  exit(0);
}

void processCleanInput(std::vector<std::string>& cleanInput, std::string& loggedInUsername, 
Requester& req, std::string &session) {
  std::string command = cleanInput[0];
  if (!command.compare("create")) {
    handleCreateHost(cleanInput, loggedInUsername, req, session);
  } else if (!command.compare("login")) {
    handleLoginHost(cleanInput, loggedInUsername, req, session);
  } else if (!command.compare("help")) {
    displayHelp();
  } else if (!command.compare("logout")) {
    handleLogoutHost(loggedInUsername, session);
  } else if (!command.compare("exit")) {
    handleExit();
  }
};

int validateCommandLinePort(std::string arg) {
  // Lifted error checking code from
  // https://stackoverflow.com/questions/2797813/how-to-convert-a-command-line-argument-to-int
  try {
    std::size_t pos;
    int port = std::stoi(arg, &pos);
    if (pos < arg.size()) {
      std::cerr << "Trailing characters after port number: " << arg << '\n';
      handleExit();
    }
    if (port < 0 || port > 65535) {
      std::cerr << "Invalid port number: " << arg << '\n';
      handleExit();
    }
    return port;
  } catch (std::invalid_argument const &ex) {
    std::cerr << "Invalid number format: " << arg << '\n';
    handleExit();
  } catch (std:: out_of_range const &ex) {
    std::cerr << "Number out of range of possible integers: " << arg << '\n';
    handleExit();
  }
  return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Insufficient command line arguments.\n";
    std::cout << "Start client in the format: ./client <port>\n";
    handleExit();
  }
  std::string arg = argv[1];
  int port = validateCommandLinePort(arg);
  std::string userInput;
  std::vector<std::string> cleanInput;
  Parser clientParser;
  Requester req;
  std::string loggedInUsername;
  std::string session;
  std::cout << "Welcome to Project Tiger!\n";
  std::cout << "A list of commands can be displayed by typing 'help' and hitting enter.\n";
  std::cout << "Otherwise you can begin entering commands.\n";
  while (true) {
    std::cout << ">" << " ";
    getline(std::cin, userInput);
    int validUserInput = clientParser.verifyInput(userInput);
    if (validUserInput) {
      cleanInput = clientParser.getCleanInput();
      processCleanInput(cleanInput, loggedInUsername, req, session);
    } else {
      std::cout << "Invalid command. Type 'help' and hit enter to see a list of valid commands.\n";
    }
  }
  return EXIT_SUCCESS;
}
