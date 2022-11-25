#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sqlite3.h>
//#include "../libraries/sqlite/sqlite3.h"
#include "sql.h"


// Method used for printing data. Used for the selectData() method.
static int callback(void* NotUsed, int argc,
  char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        // column name and value
        std::cout << azColName[i] << ": " << argv[i] << std::endl;
    }

    std::cout << "finish\n";

    return 0;
}

// static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static int countCallback(void *count, int argc, char **argv, char **azColName) {
    int *c = reinterpret_cast<int *>(count);
    ++*c;
    return 0;
}


static int intCallback(void *intPointer, int argc, char**argv,
    char **azColName) {
        if (!argv[0]) {
          //empty column
          std::cout << azColName[0] << " is null, no records inserted so far\n";
        } else {
          int *mostWon = reinterpret_cast<int *>(intPointer);
          std::cout << azColName[0] << ": " << argv[0] << "\n";
          *mostWon = std::stoi(argv[0]);
        }
        return 0;
}

Database::Database(const char* db_dir) {
    directory = db_dir;
    // Ideally, we would not have the database always-open, but this approach
    // is safer and more reliable for our use case.
    sqlite3_open(directory, &DB);

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
        "game_id     INT       NOT NULL, "
        "game_type   TEXT      NOT NULL, "
        "username    CHAR(50)  NOT NULL, "
        "player_id   CHAR(50)  NOT NULL, "
        "result      TEXT      NOT NULL, "
        "earning     INT       NOT NULL, "
        "CONSTRAINT game_host PRIMARY KEY (game_id, username) );";

    this->createTable(command2);

    std::string command3 = "CREATE TABLE IF NOT EXISTS achievements("
        "player_id          CHAR(50)  NOT NULL, "
        "username           CHAR(50)  NOT NULL, "
        "achievement_id     INT       NOT NULL, "
        "description        TEXT      NOT NULL, "
        "unlocked           INT       NOT NULL DEFAULT 0, "
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
        "pw_hash CHAR(50) NOT NULL );";

    this->createTable(command5);

    std::string command6 = "CREATE TABLE IF NOT EXISTS games("
        "game_name CHAR(50) NOT NULL PRIMARY KEY );";

    this->createTable(command6);

    the_Statement = NULL;
}

Database::~Database() {
  sqlite3_close(DB);
}

// Method for creating tables in the database.
int Database::createTable(std::string command) {
    char* messageError = 0;

    try {
        /* An open database, SQL to be evaluated, 
		Callback function, 1st argument to callback, Error msg written here */
        int exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            std::cerr << "ERROR_CODE: " << exit <<
              ", Error in createTable function." << std::endl;
            sqlite3_free(messageError);
            return -1;
        } else {
            // std::cout << "Table created Successfully" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
    }

    return 0;
}

// Method that return sqlite statement, given SQL command.
// sqlite statements are used for return table values.
sqlite3_stmt* Database::makeStatement(std::string command) {
    sqlite3_prepare_v2(DB, command.c_str(), -1, &the_Statement, 0);
    sqlite3_reset(the_Statement);
    return the_Statement;
}

int Database::totalRows(std::string command) {
    char* messageError = 0;
    int count = 0;
    int exit = sqlite3_exec(DB, command.c_str(), countCallback, &count,
      &messageError);
    if (exit != SQLITE_OK) {
      std::cerr << "ERROR_CODE: " << exit <<
        ", Error when getting total rows\n";
      sqlite3_free(messageError);
      return -1;
    }
    return count;
}

int Database::getIntValue(std::string command) {
    char *messageError = 0;
    int value = 0;
    int exit = sqlite3_exec(DB, command.c_str(), intCallback, &value,
        &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "ERROR_CODE: " << exit <<
          ", Error when getting int value\n";
        sqlite3_free(messageError);
        return -1;
    }
    return value;
}

std::string Database::getTextValue(std::string command) {
    sqlite3_stmt* queryResult = makeStatement(command);
    int exit = sqlite3_step(queryResult);
    if (exit != SQLITE_ROW) {
        std::cerr << exit << "\n";
        std::cerr << "Error when getting text value\n";
        return "-1";
    }
    const unsigned char* value = sqlite3_column_text(queryResult, 0);
    std::string result = std::string(reinterpret_cast<const char *>(value));
    return result;
}

// Method that checks if table is empty.
int Database::doesExist(sqlite3_stmt* statement) {
    //sqlite3_step(statement);
    int test = sqlite3_column_int(statement, 0);
    if (test > 0) {
        std::cout << "TEST1 PLEASE = " << test << std::endl;
        sqlite3_reset(statement);
        return 1;
    } else {
        std::cout << "TEST2 PLEASE = " << test << std::endl;
        return 0;
    }
    /*
    int check = sqlite3_step(statement);
    std::cout << "CHECKING PLEASE -- " << check << std::endl;
    sqlite3_reset(statement);
    
    if (sqlite3_step(statement) != SQLITE_DONE) {
        sqlite3_reset(statement);
        return 1;
    } else {
        return 0;
    }*/
}/**/

// Method to execute a given SQL command
int Database::executeCommand(std::string command, std::string errMsg,
 std::string successfulMessage, int theType) {
    char* messageError = 0;
    int exit;
    /* An open database, SQL to be evaluated, 
	Callback function, 1st argument to callback, Error msg written here */

    if (theType == 1) {
        exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
    } else if (theType == 0) {
        exit = sqlite3_exec(DB, command.c_str(), callback, NULL, &messageError);
    } else {
        std::cerr << "Error in SQL execute: Only type 1 and 0 should exist!"
          << std::endl;
        std::cerr << errMsg << std::endl;
        return -1;
    }

    if (exit == 19) {
        std::cerr << "ERROR_CODE: 19, Constraint/Duplicate Error!" <<
          std::endl;
        sqlite3_free(messageError);
        return 0;
    } else if (exit != SQLITE_OK) {
        std::cerr << "ERROR_CODE: " << exit << ", " <<
          errMsg << ": " << messageError << std::endl;
        sqlite3_free(messageError);
        return -1;
    } else {
        std::cout << successfulMessage << std::endl;
    }

    //std::cout << successfulMessage << std::endl;
    return 0;
}

// Method for inserting data into specific tables
int Database::insertData(std::string command) {
    int run = executeCommand(command, "Error in insertData function.",
    "Records inserted Successfully!", 1);
    return run;
}
// Method to update table data, given SQL command.
int Database::updateData(std::string command) {
    int run = executeCommand(command, "Error in updateData function.",
    "Records updated Successfully!", 1);
    return run;
}

// Method to select and look at data, given SQL command
int Database::selectData(std::string command) {
    int run = executeCommand(command, "Error in selectData function.",
    "Records selected Successfully!", 0);
    return run;
}

// Method to delete data, given SQL command.
int Database::deleteData(std::string command) {
    int run = executeCommand(command, "Error in deleteData function.",
    "Records deleted Successfully!", 0);
    return run;
}

// Method to drop table, given SQL command, Alex Brebenel version
int Database::dropTable2(std::string command) {
    int run = executeCommand(command, "Error in deleteData2 function.",
    "Records deleted Successfully!", 0);
    return run;
}
