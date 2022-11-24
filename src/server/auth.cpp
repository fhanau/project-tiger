#include <string>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <crow.h>
#include "auth.h"
#include "../sqliteDB/sql.h"

std::string tigerAuth::createUniqueToken() {
  static std::string token = "";
  if (token.compare("")) {
    return token;
  }
  unsigned char token_buf[TOKEN_BYTES];
  int error = RAND_bytes(token_buf, TOKEN_BYTES);
  // RAND_bytes may fail if the OS runs out of random data – this is
  // unlikely enough to disregard for this project.
  (void)error;
  // Use modified base64 encoding to have an ASCII token
  token = crow::utility::base64encode_urlsafe(token_buf, TOKEN_BYTES);
  //db.addNewClient(token);
  return token;
}

int tigerAuth::getUserID(const std::string& clientToken) {
  std::string query = "";
  // TODO: return -1 in case of error, otherwise return authorized user ID
  return 0;
}

std::string tigerAuth::get_hash(const std::string& password) {
  unsigned char sha_digest[SHA256_DIGEST_LENGTH];
  SHA256((const unsigned char *)password.c_str(), password.size(), sha_digest);
  std::string hash = crow::utility::base64encode_urlsafe(sha_digest,
      SHA256_DIGEST_LENGTH);
  return hash;
}

