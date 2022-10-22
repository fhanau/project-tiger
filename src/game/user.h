#ifndef SRC_GAME_USER_H_
#define SRC_GAME_USER_H_

#include <string>

class User {
 private:
    std::string username;
    // TODO: add other fields 

 public:
    User();
    User(std::string username);
    ~User();
};

#endif  // SRC_GAME_USER_H_