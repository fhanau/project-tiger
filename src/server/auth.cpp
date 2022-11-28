#include <string>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <crow/utility.h>
#include "auth.h"
#include "../sqliteDB/sql.h"

std::string tigerAuth::createUniqueToken(Database& db) {
  unsigned char token_buf[TOKEN_BYTES];
  int error = RAND_bytes(token_buf, TOKEN_BYTES);
  // RAND_bytes may fail if the OS runs out of random data – this is
  // unlikely enough to disregard for this project.
  (void)error;
  // Use modified base64 encoding to have an ASCII token
  std::string token = crow::utility::base64encode_urlsafe(token_buf,
      TOKEN_BYTES);
  // Add token to database
  db.addNewClient(get_hash(token));
  return token;
}

int tigerAuth::getAccountID(Database& db, const std::string& clientToken) {
  return db.query_token(get_hash(clientToken));
}

std::string tigerAuth::get_hash(const std::string& password) {
  unsigned char sha_digest[SHA256_DIGEST_LENGTH];
  SHA256((const unsigned char *)password.c_str(), password.size(), sha_digest);
  std::string hash = crow::utility::base64encode_urlsafe(sha_digest,
      SHA256_DIGEST_LENGTH);
  return hash;
}

