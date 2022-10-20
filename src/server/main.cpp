#include <iostream>
#include <memory>
#include <string>
#include <set>

#include "crow.h"
#include "util.h"

int main(int argc, char** argv) {
  crow::SimpleApp app;

  // Create a unique session id based on the app object's location in memory
  // Give this to clients as proof they logged in successfully

  CROW_ROUTE(app, "/")([]() {
    return "Welcome to Project Tiger";
  });

  CROW_ROUTE(app, "/create/<string>/<string>").methods(crow::HTTPMethod::GET)
  ([](std::string username, std::string password) {
    // This is where SQLWrapper.usernameExists() would be called
    int usernameExists = 0;
    if (usernameExists) {
      return crow::response("Error: Username is already taken");
    }

    // This is where the Authenticator.create() method is called
    int isSuccessful = 1;
    if (isSuccessful) {
      std::cout << "Returned successful session: " << getSession() << "\n";
      return crow::response(getSession());
    } else {
      return crow::response("");
    }
  });

  CROW_ROUTE(app, "/login/<string>/<string>")([] (std::string username,
  std::string password) {
    // This is where SQLWrapper.isValidHost() would be called
    int isValidLogin = 1;
    if (isValidLogin) {
      // Token required to access specific information
      return crow::response(getSession());
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

  CROW_ROUTE(app, "/gametype/<string>/<string>")([] (std::string type, 
  std::string sessionId) {
    if (sessionId.compare(getSession()) != 0) {
      return "ERROR: Not logged in.";
    }

    // SQLWrapper.createGameType(type): SQLWrapper.insert(type)


    return "Successfully added gametype.";
  });

  app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
