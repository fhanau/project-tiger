#ifndef SRC_SERVER_UTIL_H_
#define SRC_SERVER_UTIL_H_
#include <iostream>
#include <memory>
#include <string>
#include <set>
#include "../sqliteDB/sql.h"

// Returns singleton class for Database
Database getDatabase();

// Returns unique memory location as string to client
// Serves as token for verifying that client is logged in
std::string getSession();

//Calculates the SHA256 hash of a password using openssl and returns its base64
//representation.
std::string get_hash(const std::string& password);

#define TOKEN_BITS 256
#define TOKEN_BYTES (TOKEN_BITS / 8)
//this will change based on TOKEN_BITS, but is non-trivial to calculate
#define TOKEN_BASE64_LEN 44

#endif // SRC_SERVER_UTIL_H_
