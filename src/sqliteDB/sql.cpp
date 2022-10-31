#include <stdio.h>
#include <iostream>
#include <cstring>
#include "../libraries/sqlite/sqlite3.h"
#include "sql.h"

// static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static int countCallback(void *count, int argc, char **argv, char **azColName) {
    int *c = reinterpret_cast<int *>(count);
    ++*c;
    return 0;
}


static int intCallback(void *intPointer, int argc, char**argv,
    char**azColName) {
        int *mostWon = reinterpret_cast<int *>(intPointer);
        std::cout << azColName[0] << ": " << argv[0] << "\n";
        *mostWon = std::stoi(argv[0]);
        return 0;
}

static int textCallback(void *stringPointer, int argc, char**argv,
  char**azColName) {
    for (int i = 0; i < argc; i++) {
        // column name and value
        std::cout << azColName[i] << ": " << argv[i] << std::endl;
    }
    char *textPointer = (char *)stringPointer;
    int textLength = sizeof(argv[0]);
    if (textLength > 100) {
        std::cout << "Size of pointer before realloc: " << textLength << "\n";
        textPointer = (char *)realloc(textPointer, textLength);
    }
    strncpy(textPointer, argv[0], textLength);
    return 0;
}

Database::Database(const char* db_dir) {
    directory = db_dir;

    // Create the 6 tables named:
    // player_stats, game_list, achievements,
    // players, hosts, games.
    std::string command1 = "CREATE TABLE IF NOT EXISTS player_stats("
        "player_id      CHAR(50)    NOT NULL, "
        "username       CHAR(50)    NOT NULL, "
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
        "player_id      CHAR(50)  NOT NULL, "
        "result                 TEXT NOT NULL, "
        "earning              INT  NOT NULL, "
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

// Method for creating tables in the database.
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

// Method for inserting data into specific tables
int Database::insertData(std::string command) {
    char* messageError;

    int exit = sqlite3_open(directory, &DB);
    /* An open database, SQL to be evaluated, 
	Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in insertData function." << std::endl;
        std::cerr << messageError << "\n";
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

// Method to update table data, given SQL command.
int Database::updateData(std::string command) {
    char* messageError;

    int exit = sqlite3_open(directory, &DB);
    /* An open database, SQL to be evaluated, 
	Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in updateData function." << std::endl;
        std::cerr << messageError << "\n";
        sqlite3_free(messageError);
    } else {
        std::cout << "Records updated Successfully!" << std::endl;
    }

    return 0;
}

// Method to delete data, given SQL command.
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

int Database::dropTable(std::string command) {
    char* messageError;

    int exit = sqlite3_open(directory, &DB);
    /* An open database, SQL to be evaluated, 
	Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, command.c_str(), callback, NULL, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in dropTable function." << std::endl;
        sqlite3_free(messageError);
    } else {
        std::cout << "Table reomved Successfully!" << std::endl;
    }

    return 0;
}

// Method that return sqlite statement, given SQL command.
// sqlite statements are used for return table values.
sqlite3_stmt* Database::makeStatement(std::string command) {
    int exit = sqlite3_open(directory, &DB);
    exit = sqlite3_prepare_v2(DB, command.c_str(), -1, &the_Statement, 0);
    sqlite3_reset(the_Statement);
    return the_Statement;
}

// Method that returns the maximum value of a column of a table.
int Database::getMax(std::string table_name, std::string col_name) {
    std::string command = "SELECT MAX(" + col_name + ") FROM " + table_name;
    sqlite3_stmt* stmt = makeStatement(command);
    sqlite3_step(stmt);

    int the_max = sqlite3_column_int(stmt, 0);
    return the_max;
}

int Database::totalRows(std::string command) {
    char* messageError;
    int count = 0;
    int exit = sqlite3_open(directory, &DB);
    exit = sqlite3_exec(DB, command.c_str(), countCallback, &count,
      &messageError);
    if (exit != SQLITE_OK) {
      std::cerr << "Error when getting total rows\n";
      sqlite3_free(messageError);
    }
    return count;
}

int Database::getIntValue(std::string command) {
    char *messageError;
    int value = 0;
    int exit = sqlite3_open(directory, &DB);
    exit = sqlite3_exec(DB, command.c_str(), intCallback, &value,
        &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error when getting int value\n";
        sqlite3_free(messageError);
    }
    return value;
}

std::string Database::getTextValue(std::string command) {
    char *messageError;
    char *value = (char *)malloc(100);
    int exit = sqlite3_open(directory, &DB);
    exit = sqlite3_exec(DB, command.c_str(), intCallback, value,
        &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error when getting text value\n";
        sqlite3_free(messageError);
    }
    std::string result = std::string(value);
    free(value);
    return result;
}

// Method that checks if table is empty.
int doesExist(sqlite3_stmt* statement) {
    if (sqlite3_step(statement) != SQLITE_DONE) {
        sqlite3_reset(statement);
        return 1;
    } else {
        return 0;
    }
}

int Database::doesExist(sqlite3_stmt* statement) {
    if (sqlite3_step(statement) != SQLITE_DONE) {
        sqlite3_reset(statement);
        return 1;
    } else {
        return 0;
    }
}

// Method used for printing data. Used for the selectData() method.
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        // column name and value
        std::cout << azColName[i] << ": " << argv[i] << std::endl;
    }

    std::cout << "finish\n" << std::endl;

    return 0;
}
