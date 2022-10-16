#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>

using namespace std;

class Parser {
  public:
    int verifyInput(string input);
    vector<string> getCleanInput();
  private:
    vector<string> parseWords(string input);
    vector<string> currentInput;
};

#endif
