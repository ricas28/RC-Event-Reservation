#include <iostream>
#include <vector>

#include "Database.hpp"
#include "resultCode.hpp"
#include "../common/protocol.hpp"

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
    // User is not logged in
    return LogoutResult::NOT_LOGGED_IN;
}

UnregisterResult unregister(string &uid, string &password){
    // Check if user is registered
    if(!db->user_registered(uid))
        return UnregisterResult::NOT_REGISTERED;

    // Check if user is logged in
    if(db->user_logged_in(uid)){
        // Check if  password is correct.
        if(db->check_password(uid, password)){
            if(!db->unregister_user(uid))
                return UnregisterResult::IO_ERROR;
            return UnregisterResult::SUCCESS;
        }
        return UnregisterResult::WRONG_PASS;
    }
    // User is not logged in
    return UnregisterResult::NOT_LOGGED_IN;
}

MyEventsResult myevents(string &uid, string &password, vector<pair<string, int>> &events){
    // Check if user is logged in.
    if(!db->user_logged_in(uid))
        return MyEventsResult::NOT_LOGGED_IN;

    // Check if password is correct.
    if(!db->check_password(uid, password))
        return MyEventsResult::WRONG_PASS;

    // Get user events.
    db->get_user_events(uid, events); 
    if(events.empty())
        return MyEventsResult::NO_EVENTS_CREATED;
    return MyEventsResult::SUCCESS;
}

MyReservationsResult myreservations(string &uid, string &password, vector<Reservation> &reservations){
    // Check if user is logged in.
    if(!db->user_logged_in(uid))
        return MyReservationsResult::NOT_LOGGED_IN;

    // Check if password is correct.
    if(!db->check_password(uid, password))
        return MyReservationsResult::WRONG_PASS;

    // Get user reservations.
    db->get_user_reservations(uid, reservations); 
    if(reservations.empty())
        return MyReservationsResult::NO_RESERVATIONS_MADE;
    return MyReservationsResult::SUCCESS;
}

CreateResult create(string &uid, string &password, Event_creation_Info &event, string &eid){
    // Check if user is logged in.
    if(!db->user_logged_in(uid)){
        return CreateResult::NOT_LOGGED_IN;
    }

    // Check if password is correct.
    if(!db->check_password(uid, password))
        return CreateResult::WRONG_PASS;

    // Create event
    if(!db->create_event(uid, event, eid)){
        if(eid == "-1") 
            return CreateResult::FAILED_CREATE;
        return CreateResult::IO_ERROR;
    }
        
    return CreateResult::SUCCESS;
}

