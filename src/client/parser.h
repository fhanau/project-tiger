#ifndef SRC_CLIENT_PARSER_H
#define SRC_CLIENT_PARSER_H

#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>

// Handles wrangling client input to be used for other functions
class Parser {
  public:
    // Verifies that command entered by client is valid and recognized 
    int verifyInput(std::string input);
    // Returns most recently wrangled client input
    std::vector<std::string> getCleanInput();
  private:
    std::vector<std::string> parseWords(std::string input);
    std::vector<std::string> currentInput;
};

#endif
