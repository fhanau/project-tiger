#include <iostream>
#include <memory>
#include <string>

#include "sqlite3.h"
#include "../libraries/sqlite/sqlite3.h"

using namespace std;

class Database {
    public:
        sqlite3 *db;   
        
        Database(char* text){

            char *zErrMsg = 0;
            int rc;

            // Text should be in this format: "test.db"
            rc = sqlite3_open(text, &db);
        
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

            sqlite3_close(db);

        }
};


int main(int argc, char** argv) {




}