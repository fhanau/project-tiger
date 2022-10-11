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
    set<string> validCommands;
    string currentInput;
    vector<string> parseWords(string input);
};