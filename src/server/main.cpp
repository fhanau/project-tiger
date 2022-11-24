#include "tiger.h"

int main(int argc, char** argv) {
  // To facilitate testing and coverage, our server frontend is implemented in a
  // separate function. Check tiger.cpp for the server implementation.
  // Use db.db as the default database location.
  Tiger::runTigerServer("db.db");
  return 0;
}
