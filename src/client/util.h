#ifndef SRC_CLIENT_UTIL_H
#define SRC_CLIENT_UTIL_H
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

#include "parser.h"
#include "requester.h"

#define TIGER_CL_TOKEN_PATH "client-token.dat"

// Loads token from disk. Returns EXIT_FAILURE if token file can't be found or
// is invalid.
int load_token(std::string& token);

// Saves token to disk.
void save_token(const std::string& token);

// Sends a request to the server to get the ID associated with a token – the
// client does not need the ID, so this is only needed for debug/info purposes.
std::string get_id(Requester& req, const std::string& token);

// Sends a request to the server to create a new token.
int create_token(Requester& req, std::string& token);

// Validates input for requesting public statistics
void handlePublicStats(const std::vector<std::string> &input, Requester& req);

// Validates input for requesting private statistics
void handlePrivateStats(const std::vector<std::string> &input, Requester& req,
 const std::string &host, const std::string &session);

// Displays all available commands for private data requests for clients
void displayPrivateHelp();

// Displays all available commands for clients
void displayHelp();

// Takes client input for result description & concatenates it to one string
std::string formatResult(const std::vector<std::string> &input);

// Terminates client program
void handleExit();

// Determines command being entered by client and calls appropriate handler
void processCleanInput(const std::vector<std::string>& cleanInput,
    Requester& req, const std::string &token);

// Handle uploading game data to server
void handleUploadGameData(const std::vector<std::string> &input,
    const std::string &loggedInUsername, Requester& req,
    const std::string &session);

// Display options for private statistics requests
void displayPrivateHelp();
#endif

