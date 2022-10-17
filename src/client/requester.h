#include <string>

class Requester {
  public:
    std::string createHost(std::string username, std::string password);
    std::string loginHost(std::string username, std::string password);
  private:
    char buf[4096];
};