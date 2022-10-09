#include <iostream>
#include <memory>
#include <string>

#include "crow.h"

int main(int argc, char** argv) {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([]() {
    return "Welcome to Project Tiger";
  });

  app.port(18080).multithreaded().run();
}
