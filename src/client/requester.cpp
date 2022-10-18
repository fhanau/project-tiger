#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>

#include "requester.h"

std::string createHost(std::string username, std::string password) {
  std::string requestBody = "...";
  std::string request = requestHeader.append(requestBody);

  c.send(asio::buffer(request));
  size_t received = c.receive(asio::buffer(buf, 4096));
  std::string response(buf);
  return response;
}

std::string loginHost(std::string username, std::string password) {
  std::string requestBody = "...";
  std::string request = requestHeader.append(requestBody);
  c.send(asio::buffer(request));
  size_t received = c.receive(asio::buffer(buf, 4096));
  std::string response(buf);
  return response;
}
    