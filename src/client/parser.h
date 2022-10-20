#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>

class Parser {
  public:
    int verifyInput(std::string input);
    std::vector<std::string> getCleanInput();
  private:
    std::vector<std::string> parseWords(std::string input);
    std::vector<std::string> currentInput;
};

#endif
