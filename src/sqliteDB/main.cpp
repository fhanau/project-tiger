/*
Bobby Chapa
C++ SQLite Demo
Created on 11/20/2019
Last Edited on 11/22/2019
*/

// Final Version
// #include <sqlite3.h>

#include <stdio.h>
#include <iostream>
#include "../libraries/sqlite/sqlite3.h"

using namespace std;

static int createDB(const char* s);
static int createTable(const char* s);
static int deleteData(const char* s);
static int insertData(const char* s);
static int updateData(const char* s);
static int selectData(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);

class Database{
 private:
    /* data */
    sqlite3* DB;
    const char* directory;

 public:
    explicit Database(const char* db_dir);
    ~Database();
    int createTable(string command);
	int insertData(string command);
	int selectData(string command);
	int updateData(string command);
	int deleteData(string command);
	//int callback(void* NotUsed, int argc, char** argv, char** azColName);
};

Database::Database(const char* db_dir){
    directory = db_dir;
    //int exit = 0;
	//exit = sqlite3_open(directory, &DB);

    // Create the 3 tables
    string command1 = "CREATE TABLE IF NOT EXISTS player_stats("
		"player_id      INT  		NOT NULL, "
		"host_id      	INT  		NOT NULL, "
		"name           CHAR(50) 	NOT NULL, "
		"game_type      CHAR(50) 	NOT NULL, "
		"total_wins     INT  		NOT NULL, "
        "total_losses   INT  		NOT NULL, "
        "most_won       INT  		NOT NULL, "
        "most_lost      INT  		NOT NULL, "
        "total_money    INT  		NOT NULL, "
		"CONSTRAINT player_host_achievement PRIMARY KEY (player_id, host_id, game_type) );";

    this->createTable(command1);

    string command2 = "CREATE TABLE IF NOT EXISTS game_list("
		"game_id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"game_type              TEXT NOT NULL, "
		"host_CHECK             TEXT NOT NULL, "
		"host_id             	INT  NOT NULL, "
		"winning_player_id      INT  NOT NULL, "
        "result                 TEXT NOT NULL, "
        "money_won              INT  NOT NULL, "
		"CONSTRAINT player_host_achievement PRIMARY KEY (game_id, host_id) );";

    this->createTable(command2);

    string command3 = "CREATE TABLE IF NOT EXISTS achievements("
		"player_id          INT  NOT NULL, "
		"host_id          	INT  NOT NULL, "
		"achievement_id     INT  NOT NULL, "
		"description        TEXT NOT NULL, "
		"unlocked           INT  NOT NULL DEFAULT 0, "
		"CONSTRAINT player_host_achievement PRIMARY KEY (player_id, host_id, achievement_id) );";
    
    this->createTable(command3);

    string command4 = "CREATE TABLE IF NOT EXISTS players("
		"player_id  INT	  	 NOT NULL, "
		"host_id    INT  	 NOT NULL, "
		"name       CHAR(50) NOT NULL, "
		"lname      CHAR(50) NOT NULL, "
		"CONSTRAINT player_host PRIMARY KEY (player_id, host_id) );";

	this->createTable(command4);

	string command5 = "CREATE TABLE IF NOT EXISTS hosts("
		"host_id  INTEGER PRIMARY KEY AUTOINCREMENT, "
		"username       CHAR(50) NOT NULL, "
		"password      CHAR(50) NOT NULL );";

	this->createTable(command5);

	string command6 = "CREATE TABLE IF NOT EXISTS games("
		"game_name  CHAR(50) NOT NULL PRIMARY KEY );";

	this->createTable(command6);

	

    //sqlite3_close(DB);
}

Database::~Database()
{
    //sqlite3_close(DB);
}

int Database::createTable(string command)
{
	//sqlite3 *DB;
	char* messageError;

	try
	{
		int exit = 0;
		exit = sqlite3_open(directory, &DB);
		/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
		exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in createTable function." << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Table created Successfully" << endl;
		sqlite3_close(DB);
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}

	return 0;
}

int Database::insertData(string command)
{
    char* messageError;
		
	int exit = sqlite3_open(directory, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertData function." << endl;
		sqlite3_free(messageError);
	}
	else{
		cout << "Records inserted Successfully!" << endl;
	}

	return 0;
}

int Database::selectData(string command)
{
	//sqlite3* DB;
	char* messageError;

	//string sql = "SELECT * FROM GRADES;";

	int exit = sqlite3_open(directory, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, command.c_str(), callback, NULL, &messageError);

	if (exit != SQLITE_OK) {
		cerr << "Error in selectData function." << endl;
		sqlite3_free(messageError);
	}
	else{
		cout << "Records selected Successfully!" << endl;
	}

	return 0;
}

int Database::updateData(string command)
{
	//sqlite3* DB;
	char* messageError;

	//string sql("UPDATE GRADES SET GRADE = 'A' WHERE LNAME = 'Cooper'");

	int exit = sqlite3_open(directory, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, command.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
	}
	else{
		cout << "Records updated Successfully!" << endl;
	}

	return 0;
}

int Database::deleteData(string command){
	
	//sqlite3* DB;
	char* messageError;

	//string sql = "DELETE FROM GRADES;";

	int exit = sqlite3_open(directory, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, command.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in deleteData function." << endl;
		sqlite3_free(messageError);
	}
	else{
		cout << "Records deleted Successfully!" << endl;
	}

	return 0;
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		// column name and value
		cout << azColName[i] << ": " << argv[i] << endl;
	}

	cout << "finish\n" << endl;

	return 0;
}

int main()
{
	const char* dir = "test.db";

    Database dummy("dummy.db");

    dummy.insertData("INSERT INTO player_stats (player_id, host_id, name, game_type,total_wins, total_losses, \
        most_won, most_lost, total_money) VALUES(100, 500, 'Alex', 'RPS', 35, 53, 10000000, 50, 2222);");

	dummy.selectData("SELECT * FROM player_stats WHERE player_id = 100;");

	dummy.updateData("UPDATE player_stats SET player_id = 102 WHERE player_id = 100;");

	dummy.deleteData("DELETE FROM player_stats WHERE player_id = 100;");

    /*
	createDB(dir);
	createTable(dir);
	//deleteData(dir); // use prior to insert
	insertData(dir); // uncomment the deleteData above to avoid duplicates
	updateData(dir);
	selectData(dir);*/

	return 0;
}

static int createDB(const char* s)
{
	sqlite3* DB;
	
	int exit = 0;
	exit = sqlite3_open(s, &DB);

	//sqlite3_close(DB);

	return 0;
}

static int createTable(const char* s)
{
	sqlite3 *DB;
	char* messageError;

	string sql = "CREATE TABLE IF NOT EXISTS GRADES("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME      TEXT NOT NULL, "
		"LNAME     TEXT NOT NULL, "
		"AGE       INT  NOT NULL, "
		"ADDRESS   CHAR(50), "
		"GRADE     CHAR(1) );";

	try
	{
		int exit = 0;
		exit = sqlite3_open(s, &DB);
		/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in createTable function." << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Table created Successfully" << endl;
		sqlite3_close(DB);
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}

	return 0;
}

static int insertData(const char* s)
{
	sqlite3* DB;
	char* messageError;
		
	string sql("INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES('Alice', 'Chapa', 35, 'Tampa', 'A');"
		"INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES('Bob', 'Lee', 20, 'Dallas', 'B');"
		"INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES('Fred', 'Cooper', 24, 'New York', 'C');");

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records inserted Successfully!" << endl;

	return 0;
}

static int updateData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql("UPDATE GRADES SET GRADE = 'A' WHERE LNAME = 'Cooper'");

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records updated Successfully!" << endl;

	return 0;
}

static int deleteData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "DELETE FROM GRADES;";

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in deleteData function." << endl;
		sqlite3_free(messageError);
	}
	else{
		cout << "Records deleted Successfully!" << endl;
	}

	return 0;
}

static int selectData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "SELECT * FROM GRADES;";

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);

	if (exit != SQLITE_OK) {
		cerr << "Error in selectData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records selected Successfully!" << endl;

	return 0;
}

// retrieve contents of database used by selectData()
/* argc: holds the number of results, argv: holds each value in array, azColName: holds each column returned in array, */
static int callback1(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		// column name and value
		cout << azColName[i] << ": " << argv[i] << endl;
	}

	cout << "finish\n" << endl;

	return 0;
}




/*
#include <iostream>
#include <memory>
#include <string>

#include "sqlite3.h"
//#include "sqlite3ext.h"
//#include "../libraries/sqlite/sqlite3.h"


static int callback(void* NotUsed, int argc, char** argv, char** azColName);

using namespace std;

class Database {
    public:
        sqlite3 *db;   
        
        Database(string text){

            char *zErrMsg = 0;
            int rc;

            // Text should be in this format: "test.db"
            rc = sqlite3_open(text.c_str(), &db);
        
            if( rc ) 
            {
                fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
            }
            else 
            {
                fprintf(stderr, "Opened database successfully\n");
            }
        }

        ~Database(){

            cout << "Database closed" << endl;
            sqlite3_close(db);

        }

        void execute_sql(string command){

            char* messageError;
            sqlite3_exec(db, command.c_str(), callback, NULL, &messageError); //NULL, 0, NULL); //&messageError);
            cout << "It worked" << endl;

        }
};



int main(int argc, char** argv) {


    Database dummyDB("test.db");

	string sql = "CREATE TABLE IF NOT EXISTS GRADES("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME      TEXT NOT NULL, "
		"LNAME     TEXT NOT NULL, "
		"AGE       INT  NOT NULL, "
		"ADDRESS   CHAR(50), "
		"GRADE     CHAR(1) );";

    dummyDB.execute_sql(sql);

    string sql2("INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES('Alice', 'Chapa', 35, 'Tampa', 'A');"
		"INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES('Bob', 'Lee', 20, 'Dallas', 'B');"
		"INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES('Fred', 'Cooper', 24, 'New York', 'C');");

    dummyDB.execute_sql(sql2);

    string sql3("SELECT * FROM GRADES);");

    cout << "print stuff" << endl;

    dummyDB.execute_sql(sql3);

    string x;
    bool loop = true;
    while(loop)
    {

        cout << "Command or q:";
        cin >> x;
        cout << "" << endl;


        if(x == "command")
        {
            cout << "SQL Command:";
            cin >> x;
            dummyDB.execute_sql(x);
        }
        else if (x == "q")
        {
            loop = false;
            cout << "You have finished writing commands" << endl;
            cout << "Database will close now" << endl;
            break;
        }

    }

	return 0;


}

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		// column name and value
		cout << azColName[i] << ": " << argv[i] << endl;
	}

	cout << endl;

	return 0;
} 
*/
