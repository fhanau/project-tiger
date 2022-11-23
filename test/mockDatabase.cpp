#include "mockDatabase.h"

int MockDatabase::insertMockData(std::string field, std::string data) {
  if (!field.compare("hosts")) {
    hosts.insert(data);
    return 1;
  } else if (!field.compare("tokens")) {
    tokens.insert(data);
    return 1;
  }
  return 0;
}

MockDatabase::MockDatabase() {
  std::cout << "Created database";
}

int MockDatabase::totalMockRows(std::string field, std::string data) {
  if (!field.compare("hosts")) {
    return hosts.find(data) != hosts.end();
  } else if (!field.compare("tokens")) {
    return tokens.find(data) != tokens.end();
  }
  return 0;
}

crow::SimpleApp MockDatabase::getMockApp() {
  return mockApp;
}
