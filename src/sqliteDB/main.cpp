
#include <stdio.h>
#include <iostream>
#include "../libraries/sqlite/sqlite3.h"

static int callback(void* NotUsed, int argc, char** argv, char** azColName);

class Database{
 private:
    sqlite3* DB;
    const char* directory;

 public:
    explicit Database(const char* db_dir);
    ~Database();
    int createTable(std::string command);
    int insertData(std::string command);
    int selectData(std::string command);
    int updateData(std::string command);
    int deleteData(std::string command);
    // int callback(void* NotUsed, int argc, char** argv, char** azColName);
};

Database::Database(const char* db_dir) {
    directory = db_dir;

    // Create the 6 tables
    std::string command1 = "CREATE TABLE IF NOT EXISTS player_stats("
        "player_id      INT         NOT NULL, "
        "host_id        INT         NOT NULL, "
        "name           CHAR(50)    NOT NULL, "
        "game_type      CHAR(50)    NOT NULL, "
        "total_wins     INT         NOT NULL, "
        "total_losses   INT         NOT NULL, "
        "most_won       INT         NOT NULL, "
        "most_lost      INT         NOT NULL, "
        "total_money    INT         NOT NULL, "
        "CONSTRAINT player_host_achievement PRIMARY KEY (player_id, host_id, game_type) );";

    this->createTable(command1);

    std::string command2 = "CREATE TABLE IF NOT EXISTS game_list("
        "game_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "game_type              TEXT NOT NULL, "
        "host_CHECK             TEXT NOT NULL, "
        "host_id                INT  NOT NULL, "
        "winning_player_id      INT  NOT NULL, "
        "result                 TEXT NOT NULL, "
        "money_won              INT  NOT NULL, "
        "CONSTRAINT player_host_achievement PRIMARY KEY (game_id, host_id) );";

    this->createTable(command2);

    std::string command3 = "CREATE TABLE IF NOT EXISTS achievements("
        "player_id          INT  NOT NULL, "
        "host_id            INT  NOT NULL, "
        "achievement_id     INT  NOT NULL, "
        "description        TEXT NOT NULL, "
        "unlocked           INT  NOT NULL DEFAULT 0, "
        "CONSTRAINT player_host_achievement PRIMARY KEY (player_id, host_id, achievement_id) );";

    this->createTable(command3);

    std::string command4 = "CREATE TABLE IF NOT EXISTS players("
        "player_id  INT      NOT NULL, "
        "host_id    INT      NOT NULL, "
        "name       CHAR(50) NOT NULL, "
        "lname      CHAR(50) NOT NULL, "
        "CONSTRAINT player_host PRIMARY KEY (player_id, host_id) );";

    this->createTable(command4);

    std::string command5 = "CREATE TABLE IF NOT EXISTS hosts("
        "host_id  INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username CHAR(50) NOT NULL, "
        "password CHAR(50) NOT NULL );";

    this->createTable(command5);

    std::string command6 = "CREATE TABLE IF NOT EXISTS games("
        "game_name CHAR(50) NOT NULL PRIMARY KEY );";

    this->createTable(command6);
}

Database::~Database() {}

int Database::createTable(std::string command) {
    char* messageError;

    try {
        int exit = 0;
        exit = sqlite3_open(directory, &DB);
        /* An open database, SQL to be evaluated, 
		Callback function, 1st argument to callback, Error msg written here */
        exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            std::cerr << "Error in createTable function." << std::endl;
            sqlite3_free(messageError);
        } else {
            std::cout << "Table created Successfully" << std::endl;
        }
        sqlite3_close(DB);
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
    }

    return 0;
}

int Database::insertData(std::string command) {
    char* messageError;

    int exit = sqlite3_open(directory, &DB);
    /* An open database, SQL to be evaluated, 
	Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in insertData function." << std::endl;
        sqlite3_free(messageError);
    } else {
        std::cout << "Records inserted Successfully!" << std::endl;
    }

    return 0;
}

int Database::selectData(std::string command) {
    char* messageError;

    int exit = sqlite3_open(directory, &DB);
    /* An open database, SQL to be evaluated, 
	Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, command.c_str(), callback, NULL, &messageError);

    if (exit != SQLITE_OK) {
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    } else {
        std::cout << "Records selected Successfully!" << std::endl;
    }

    return 0;
}

int Database::updateData(std::string command) {
    char* messageError;

    int exit = sqlite3_open(directory, &DB);
    /* An open database, SQL to be evaluated, 
	Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in updateData function." << std::endl;
        sqlite3_free(messageError);
    } else {
        std::cout << "Records updated Successfully!" << std::endl;
    }

    return 0;
}

int Database::deleteData(std::string command) {
    char* messageError;

    int exit = sqlite3_open(directory, &DB);
    /* An open database, SQL to be evaluated, 
	Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, command.c_str(), callback, NULL, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in deleteData function." << std::endl;
        sqlite3_free(messageError);
    } else {
        std::cout << "Records deleted Successfully!" << std::endl;
    }

    return 0;
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        // column name and value
        std::cout << azColName[i] << ": " << argv[i] << std::endl;
    }

    std::cout << "finish\n" << std::endl;

    return 0;
}

int main() {
    Database dummy("dummy.db");

    dummy.insertData("INSERT INTO player_stats(player_id, host_id, name, \
game_type, total_wins, total_losses, most_won, most_lost, total_money) VALUES(100, 500, 'Alex', 'RPS', 35, 53, 10000000, 50, 2222); ");

    dummy.selectData("SELECT * FROM player_stats WHERE player_id = 100;");

    dummy.updateData("UPDATE player_stats SET player_id = 102 WHERE player_id = 100;");

    dummy.deleteData("DELETE FROM player_stats WHERE player_id = 100;");

    return 0;
}
