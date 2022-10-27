#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <cstring>

#include "parser.h"

int Parser::verifyInput(std::string input) {
  std::set<std::string> validCommands = {"create", "login", "help", "logout",
    "exit", "upload", "public"};
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
  char *token = strtok(const_cast<char*>(input.c_str()), delimiter);
  while (token != nullptr) {
      words.push_back(std::string(token));
      token = strtok(nullptr, delimiter);
  }
  return words;
}
