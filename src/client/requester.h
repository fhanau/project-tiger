#ifndef REQUESTER_H
#define REQUESTER_H

#include <string>
#include "crow.h"

class Requester {
  public:
    std::string createHost(std::string username, std::string password);
    std::string loginHost(std::string username, std::string password);
    inline Requester(int port) {
      client.bindaddr("127.0.0.1").port(port).run();
      c.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), port));
    }
  private:
    char buf[4096];
    std::string requestHeader = "GET";
    crow::SimpleApp client;
    asio::io_service is;
    asio::ip::tcp::socket c(is);
};

#endif