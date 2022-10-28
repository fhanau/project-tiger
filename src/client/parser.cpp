#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <cstring>

#include "parser.h"

int Parser::verifyInput(std::string input) {
  std::set<std::string> validCommands = {"create", "login", "help", "logout",
    "exit", "gametype", "upload", "public", "private", "help-private"};
  std::vector<std::string> words = parseWords(input);
  if (words.size() < 1) {
    return 0;
  }
  std::string command = words[0];
  if (validCommands.find(command) != validCommands.end()) {
    currentInput = words;
    return 1;
  } else {
    currentInput.clear();
    return 0;
  }
}

std::vector<std::string> Parser::getCleanInput() {
  return currentInput;
}

std::vector<std::string> Parser::parseWords(std::string input) {
  const char* delimiter = " ";
  std::vector<std::string> words;
  std::string temp;
  std::istringstream ss(input);
  while (std::getline(ss, temp, ',')) {
      words.push_back(temp);
  }
  return words;
}
