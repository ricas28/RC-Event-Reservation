#include <iostream>

#include "Database.hpp"
#include "resultCode.hpp"

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

LoginResult login(string &uid, string &password){
    // If user exists, check if password is correct.
    if(db->user_registered(uid)){
        if(!db->check_password(uid, password))
            return LoginResult::WRONG_PASS;
        db->login_user(uid);
        return LoginResult::SUCCESS;
    }   

    // Register user.
    if(!db->register_user(uid, password)){
        return LoginResult::IO_ERROR;
    }
    return LoginResult::REGISTERED;
}

LogoutResult logout(string &uid, string &password){
    // Check if user is registered
    if(!db->user_registered(uid))
        return LogoutResult::NOT_REGISTERED;

    // Check if user is logged in
    if(db->user_logged_in(uid)){
        // Check if  password is correct.
        if(db->check_password(uid, password)){
            if(!db->logout_user(uid))
                return LogoutResult::IO_ERROR;
            return LogoutResult::SUCCESS;
        }
        return LogoutResult::WRONG_PASS;
    }
    // User is not not logged in
    return LogoutResult::NOT_LOGGED_IN;
}

