#include <iostream>
#include <memory>
#include <string>

#include "crow.h"

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

  app.port(18080).multithreaded().run();
}
