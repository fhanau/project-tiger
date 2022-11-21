#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <crow.h>
#include "util.h"
#include "../sqliteDB/sql.h"

Database getDatabase() {
  static Database sql("db.db");
  return sql;
}

std::string getSession() {
  static std::string session = "";
  if (session.compare("")) {
    return session;
  }
  unsigned char token_buf[TOKEN_BYTES];
  int error = RAND_bytes(token_buf, TOKEN_BYTES);
  //RAND_bytes may fail if the OS runs out of random data – this is
  //unlikely enough to disregard for this project.
  (void)error;
  session = crow::utility::base64encode(token_buf, TOKEN_BYTES);
  //TODO: Add to token database
  return session;
}

std::string get_hash(const std::string& password) {
  unsigned char sha_digest[SHA256_DIGEST_LENGTH];
  SHA256((const unsigned char *)password.c_str(), password.size(), sha_digest);
  std::string hash = crow::utility::base64encode(sha_digest,
      SHA256_DIGEST_LENGTH);
  return hash;
}

