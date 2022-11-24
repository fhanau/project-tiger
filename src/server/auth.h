#ifndef SRC_SERVER_AUTH_H_
#define SRC_SERVER_AUTH_H_
#include <string>

//TODO: To be deleted
#define getSession tigerAuth::createUniqueToken
namespace tigerAuth {
// Returns a new unique token to identify a client and adds it to the database.
// Clients can use tokens to identify themselves to the server. Note that as a
// side effect of having unique tokens for verification, the function
// necessarily returns a different value every time.
std::string createUniqueToken();

// Calculates the SHA256 hash of a password using openssl and returns its base64
// representation.
std::string get_hash(const std::string& password);

// Return the server-internal user ID corresponding to a token.
// Return -1 if the token is invalid.
int getUserID(const std::string& clientToken);
} // namespace tigerAuth

#define TOKEN_BITS 256
#define TOKEN_BYTES (TOKEN_BITS / 8)
//this will change based on TOKEN_BITS, but is non-trivial to calculate
#define TOKEN_BASE64_LEN 44

#endif // SRC_SERVER_AUTH_H_
