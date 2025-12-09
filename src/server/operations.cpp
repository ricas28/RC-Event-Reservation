#include <iostream>

#include "Database.hpp"

using namespace std;

// Global database.
Database *db = nullptr;

int db_init(){
    db = &Database::instance();
    if(!db->is_ok()){
        cerr << "Failute to initialize DB" << endl;
        return -1;
    }
    return 0;
}

