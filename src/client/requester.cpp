#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>

// Include asio i believe
using namespace std;

class Requester {
  public:
    Requester() {
      
    }
    int createHost(string username, string password) {
      string requestBody = "...";
      string request = requestHeader.append(requestBody);
      // Send request using asio or curlpp
      string response = ""; //Get response from server
      // If our server returns a session token maybe return that instead
      return response.compare(username);
    }

    int loginHost(string username, string password) {
      string requestBody = "...";
      string request = requestHeader.append(requestBody);
      // Send request using asio or curlpp
      string response = ""; //Get response from server
      // If our server returns a session token maybe return that instead
      return response.compare(username);
    }
  private:
    char buf[4096];
    string requestHeader = "GET";
};