#include <iostream>
#include <memory>
#include <string>
#include <set>

#include "crow.h"

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

int main(int argc, char** argv) {
  crow::SimpleApp app;

  // Create a unique session id based on the app object's location in memory
  // Give this to clients as proof they logged in successfully
  uint32_t id = reinterpret_cast<uint32_t>(&app);
  char literal[10];
  sprintf(literal, "%I64u", id);
  std::string session(literal);

  CROW_ROUTE(app, "/")([]() {
    return "Welcome to Project Tiger";
  });

  CROW_ROUTE(app, "/create/<string>/<string>")([](std::string username, std::string password) {
    // This is where SQLWrapper.usernameExists() would be called
    if (true) {
      return crow::response("Error: Username is already taken");
    }

    // This is where the Authenticator.create() method is called
    int isSuccessful = 1;
    
    if (isSuccessful) {
      return crow::response(session);
    } else {
      return crow::response("");
    }
  });

  CROW_ROUTE(app, "/login/<string>/<string>")([] (std::string username, std::string password) {
    // This is where SQLWrapper.isValidHost() would be called
    // Would need to encrypt password with Authenticator.encrypt() first
    int isValidLogin = 1;
    if (isValidLogin) {
      // Could also return a session token to client as proof of successful login
      // Token would be required to access private information
      // Token could be randomly generated everytime the server is started and stored as a global variable
      return crow::response(session);
    } else {
      return crow::response("");
    }
  });

  CROW_ROUTE(app, "/public/<string>")([] (std::string type) {
    crow::json::wvalue resp({{"type", ""}});
    if (isValidTypeOfPublicRequest(type)) {
      resp["type"] = "ERROR";
      resp["errorMessage"] = "Invalid type of request for data.";
      return resp;
    }
    
    // Function for processing valid type
    std::string result = handlePublicRequest(type);
    if (!result.compare("ERROR")) {
      resp["type"] = "ERROR";
      resp["errorMessage"] = "Error when requesting public game data.";
      return resp;
    }
    resp["type"] = "public";
    resp["data"] = result;
    return resp;
  });

  app.port(18080).multithreaded().run();
}
