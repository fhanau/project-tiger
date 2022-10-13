#include <string>

using namespace std;

class Requester {
  public:
    int createHost(string username, string password);
    int loginHost(string username, string password);
  private:
    char buf[4096];
};