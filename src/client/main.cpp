#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

#include "parser.h"
#include "requester.h"
#include "util.h"

// Runs client program for accessing server
int main(int argc, char** argv) {
  // token path is configurable from the command line to better demonstrate the
  // multiple clients and persistence aspects of the server.
#define TIGER_CL_TOKEN_PATH "client-token.dat"
  std::string token_path = TIGER_CL_TOKEN_PATH;
  if (argc == 2) {
    token_path = (std::string)argv[1];
  }

  std::string userInput;
  std::vector<std::string> cleanInput;
  Parser clientParser;
  Requester req;
  std::string id = "-1";
  std::cout << "Welcome to Project Tiger!\n";
  std::string token;
  if (load_token(token, token_path) == EXIT_SUCCESS) {
    id = get_id(req, token);
    std::cout << "loaded authentication token from " << TIGER_CL_TOKEN_PATH
      << ", corresponding ID " << id << ", ready to execute commands\n";
  } else if (create_token(req, token) == EXIT_SUCCESS) {
    id = get_id(req, token);
    std::cout << "Created new token " << TIGER_CL_TOKEN_PATH << ", assigned ID "
      << id << "\n";
    save_token(token, token_path);
    std::cout << "Token saved to disk, ready to execute commands\n";
  }

  if (!id.compare("-1")) {
    std::cerr << "Failed to load token from disk or get a new token, is the "
      "server running?\n";
    return EXIT_FAILURE;
  }
  std::cout << "A list of commands can be displayed by typing 'help' and "
    "hitting enter.\n";
  std::cout << "Otherwise you can begin entering commands.\n";
  while (true) {
    std::cout << ">" << " ";
    getline(std::cin, userInput);
    // Shut down client when end of file is reached, useful for automated
    // testing
    if (std::cin.eof()) {
      break;
    }
    int validUserInput = clientParser.verifyInput(userInput);
    if (validUserInput) {
      cleanInput = clientParser.getCleanInput();
      processCleanInput(cleanInput, req, token);
    } else {
      std::cout << "Invalid command. Type 'help' and hit enter to see a list "
        "of valid commands.\n";
    }
  }
  return EXIT_SUCCESS;
}
