#ifndef SRC_CLIENT_UTIL_H
#define SRC_CLIENT_UTIL_H
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

#include "parser.h"
#include "requester.h"

// Validates input for creating host before sending request
void handleCreateHost(std::vector<std::string> &input,
    std::string& loggedInUsername, Requester& req, std::string& session);

// Validates input for logging in host before sending request
void handleLoginHost(std::vector<std::string> &input,
    std::string& loggedInUsername, Requester& req, std::string& session);

// Validates input for adding game type before sending request
void handleAddGameType(std::vector<std::string> &input,
    std::string& loggedInUsername, Requester& req, std::string& session);

// Validates input for requesting public statistics
void handlePublicStats(std::vector<std::string> &input, Requester& req);

// Validates input for requesting private statistics
void handlePrivateStats(std::vector<std::string> &input, Requester& req,
 std::string &host, std::string &session);

// Displays all available commands for private data requests for clients
void displayPrivateHelp();

// Displays all available commands for clients
void displayHelp();

// Takes client input for result description & concatenates it to one string
std::string formatResult(std::vector<std::string> &input);

// Clears loggedInUsername and sessionId variables
void handleLogoutHost(std::string& loggedInUsername, std::string& session);

// Terminates client program
void handleExit();

// Determines command being entered by client and calls appropriate handler
void processCleanInput(std::vector<std::string>& cleanInput,
    std::string& loggedInUsername, Requester& req, std::string &session);

#endif

