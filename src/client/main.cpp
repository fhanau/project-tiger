#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

#include "parser.h"
#include "requester.h"
#include "util.h"

int main(int argc, char** argv) {
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
