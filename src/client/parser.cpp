#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <cstring>

#include "parser.h"

using namespace std;

int Parser::verifyInput(string input) {
  set<string> validCommands = {"create", "login", "help", "logout", "exit"};
  vector<string> words = parseWords(input);
  if (words.size() < 1) {
        return 0;
      }
      string command = words[0];
      if (validCommands.find(command) != validCommands.end()) {
	      currentInput = words;
        return 1;
      } else {
	      currentInput.clear();
        return 0;
      }
    };
    vector<string> Parser::getCleanInput() {
      return currentInput;
    };
    vector<string> Parser::parseWords(string input) {
      const char* delimiter = " ";
      vector<string> words;
      char *token = strtok(const_cast<char*>(input.c_str()), delimiter);
    while (token != nullptr)
    {
        words.push_back(std::string(token));
        token = strtok(nullptr, delimiter);

    }
      return words;
    };
