#ifndef SRC_CLIENT_UTIL_H
#define SRC_CLIENT_UTIL_H
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

#include "parser.h"
#include "requester.h"

void handleCreateHost(std::vector<std::string> input,
    std::string& loggedInUsername, Requester& req, std::string& session);

void handleLoginHost(std::vector<std::string> input,
    std::string& loggedInUsername, Requester& req, std::string& session);

void handleAddGameType(std::vector<std::string> input,
    std::string& loggedInUsername, Requester& req, std::string& session);

void displayHelp();

std::string formatResult(std::vector<std::string> input);

void handleLogoutHost(std::string& loggedInUsername, std::string& session);

void handleExit();

void processCleanInput(std::vector<std::string>& cleanInput,
    std::string& loggedInUsername, Requester& req, std::string &session);

#endif

