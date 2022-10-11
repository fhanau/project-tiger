#include <iostream>
#include <memory>
#include <string>

#include "crow.h"

using namespace std;

int main(int argc, char** argv) {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([]() {
    return "Welcome to Project Tiger";
  });

  CROW_ROUTE(app, "/create/<string>/<string>")([](string username, string password) {
    // This is where SQLWrapper.usernameExists() would be called
    if (true) {
      return crow::response("Error: Username is already taken");
    }

    int isSuccessful = 1; // This is where the Authenicator.create() method is called
    
    if (isSuccessful) {
      return crow::response("Account creation successful");
    } else {
      return crow::response("Account creation unsuccessful. Try again.");
    }
  });

  app.port(18080).multithreaded().run();
}
