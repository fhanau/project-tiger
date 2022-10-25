#include <stdio.h>
#include <iostream>
#include "../libraries/sqlite/sqlite3.h"
#include "sql.h"

//static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static int existsCallback(void *count, int argc, char **argv, char **azColName) {
    int *c = reinterpret_cast<int *>(count);
    *c = 1;
    return 1;
}

Database::Database(const char* db_dir) {
    directory = db_dir;

    // Create the 6 tables
    // player_stats, game_list, achievements,
    // players, hosts, games.
    std::string command1 = "CREATE TABLE IF NOT EXISTS player_stats("
        "player_id      CHAR(50)    NOT NULL, "
        "username       CHAR(50)    NOT NULL, "
        "name           CHAR(50)    NOT NULL, "
        "game_type      CHAR(50)    NOT NULL, "
        "total_wins     INT         NOT NULL, "
        "total_losses   INT         NOT NULL, "
        "most_won       INT         NOT NULL, "
        "most_lost      INT         NOT NULL, "
        "total_money    INT         NOT NULL, "
        "CONSTRAINT player_host_gametype PRIMARY KEY "
        "(player_id, username, game_type) );";

    this->createTable(command1);

    std::string command2 = "CREATE TABLE IF NOT EXISTS game_list("
        "game_id                INT  NOT NULL, "
        "game_type              TEXT NOT NULL, "
        "username               CHAR(50)  NOT NULL, "
        "winning_player_id      CHAR(50)  NOT NULL, "
        "result                 TEXT NOT NULL, "
        "money_won              INT  NOT NULL, "
        "CONSTRAINT game_host PRIMARY KEY (game_id, username) );";

    this->createTable(command2);

    std::string command3 = "CREATE TABLE IF NOT EXISTS achievements("
        "player_id          CHAR(50)  NOT NULL, "
        "username           CHAR(50)  NOT NULL, "
        "achievement_id     INT  NOT NULL, "
        "description        TEXT NOT NULL, "
        "unlocked           INT  NOT NULL DEFAULT 0, "
        "CONSTRAINT player_host_achievement PRIMARY KEY "
        "(player_id, username, achievement_id) );";

    this->createTable(command3);

    std::string command4 = "CREATE TABLE IF NOT EXISTS players("
        "player_id  CHAR(50) NOT NULL, "
        "username   CHAR(50) NOT NULL, "
        "CONSTRAINT player_host PRIMARY KEY (player_id, username) );";

    this->createTable(command4);

    std::string command5 = "CREATE TABLE IF NOT EXISTS hosts("
        "username CHAR(50) PRIMARY KEY NOT NULL, "
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

sqlite3_stmt* Database::makeStatement(std::string command) {
    sqlite3_prepare_v2(DB, command.c_str(), -1, &the_Statement, 0);
    return the_Statement;
}

int Database::getMax(std::string table_name, std::string col_name) {
    std::string command = "SELECT MAX(" + col_name + ") FROM " + table_name;
    sqlite3_stmt* stmt = makeStatement(command);
    sqlite3_step(stmt);

    int the_max = sqlite3_column_int(stmt, 0);
    return the_max;
}

int Database::entryExists(std::string command) {
    int hasResult = 0;
    char* errorMsg;
    int exit = sqlite3_exec(DB, command.c_str(), existsCallback, &hasResult, &errorMsg);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in entryExists function." << std::endl;
        sqlite3_free(errorMsg);
    } 
    return hasResult;
}

int Database::totalRows(std::string command) {
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(DB, command.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in totalRows function." << std::endl;
    }
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_ROW) {
        return 0;
    }
    return sqlite3_column_int(stmt, 0);
}

/*
DELETE this comment later.
- The variable the_Statement will return the first row of the selected query.

- run step(the_Statement to return current row and point to next row after)
- SQLITE_DONE = reached end of query.
- sqlite3_column_type returns number code to signify type
    - 1 = int
    - 3 = text/char

- sqlite3_column_int(stmt, i)
- sqlite3_column_text(stmt, i)


    sqlite3_stmt* x;
    x = dummy.makeStatement("SELECT * FROM player_stats WHERE player_id = 102;");
    int col = sqlite3_column_count(x);

    while(sqlite3_step(x) != SQLITE_DONE) {
        
        for(int i = 0; i< col; i++) {
            std::cout << "Num = " << sqlite3_column_type(x, i) << std::endl;
        }

    }

*/

int doesExist(sqlite3_stmt* statement) {
    if (sqlite3_step(statement) != SQLITE_DONE) {
        sqlite3_reset(statement);
        return 1;
    } else {
        return 0;
    }
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        // column name and value
        std::cout << azColName[i] << ": " << argv[i] << std::endl;
    }

    std::cout << "finish\n" << std::endl;

    return 0;
}
