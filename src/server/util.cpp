#include <iostream>
#include <memory>
#include <string>
#include <set>


int isValidTypeOfPublicRequest(std::string type) {
  std::set<std::string> validTypes = {"games", "total"};
  return validTypes.find(type) != validTypes.end();
};

std::string requestPublicGameData() {
  // Can simplify by just having very basic information and one query for it
  // Can expand to a couple of different types of public data requests
  // Call SQLWrapper.getPublicGameData(); regardless
  // If an error occurs when quering SQL, return ERROR
  int isError = 0;
  if (isError) {
    return "ERROR";
  }
  // Would be set equal to some formatted version of the SQL data table result.
  std::string publicData = "Public Information: We store game data and crunch numbers :)";
  return publicData;
}

std::string handlePublicRequest(std::string type) {
  if (!type.compare("games")) {
    return requestPublicGameData();
  }
  // Will have to discuss what types of general information is publicly accessible
};

std::string getSession() {
  static std::string session = "";
  if (session.size() > 0) {
    return session;
  }
  uint32_t randomLocationInMemory = 1000;
  char randomLocationAsLiteral[10];
  sprintf(randomLocationAsLiteral, "%I64u", &randomLocationInMemory);
  session = std::string(randomLocationAsLiteral);
  return session;
}