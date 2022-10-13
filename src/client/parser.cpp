#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>

using namespace std;

class Parser {
  public:
    int verifyInput(string input) {
      vector<string> words = parseWords(input);
      if (words.size() < 1) {
        return 0;
      }
      string command = words.at(0);
      if (validCommands.find(command) != validCommands.end()) {
        cleanInput = words;
        return 1;
      } else {
        cleanInput = {};
        return 0;
      }
    };
    vector<string> getCleanInput() {
      return cleanInput;
    };
  private:
    set<string> validCommands = {"create", "login", "help", "logout", "exit"};
    vector<string> cleanInput = {};
    vector<string> parseWords(string input) {
      string delimiter = " ";
      vector<string> words{};
      
      size_t position = 0;
      while ((position = input.find(delimiter)) != string::npos) {
        words.push_back(input.substr(0, position));
        input.erase(0, position + 1);
      }
      
      return words;
    };
};