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
        bool equal = false;
        if(!db->check_password(uid, password, equal))
            return LoginResult::IO_ERROR;
        if(!equal)
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
        bool equal = false;
        if(!db->check_password(uid, password, equal)){
            return LogoutResult::IO_ERROR;
        }
        if(equal){
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
        bool equal = false;
        if(!db->check_password(uid, password, equal)){
            return UnregisterResult::IO_ERROR;
        }
        if(equal){
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
    bool equal = false;
    if(!db->check_password(uid, password, equal))
        return MyEventsResult::IO_ERROR;
    if(!equal)
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
    bool equal = false;
    if(!db->check_password(uid, password, equal))
        return MyReservationsResult::IO_ERROR;
    if(!equal)
        return MyReservationsResult::WRONG_PASS;

    // Get user reservations.
    db->get_user_reservations(uid, reservations); 
    if(reservations.empty())
        return MyReservationsResult::NO_RESERVATIONS_MADE;
    return MyReservationsResult::SUCCESS;
}

CreateResult create(int &sock, string &uid, string &password, Event_creation_Info &event, string &eid){
    // Check if user is logged in.
    if(!db->user_logged_in(uid)){
        return CreateResult::NOT_LOGGED_IN;
    }

    // Check if password is correct.
    bool equal = false;
    if(!db->check_password(uid, password, equal))
        return CreateResult::IO_ERROR;
    if(!equal)
        return CreateResult::WRONG_PASS;

    // Create event
    if(!db->create_event(sock, uid, event, eid)){
        if(eid == "-1")
            return CreateResult::FAILED_CREATE;
        return CreateResult::IO_ERROR;
    }
        
    return CreateResult::SUCCESS;
}

CloseResult close(string &uid, string &password, string &eid){
    // Check if user exists.
    if(!db->user_exists(uid))
        return CloseResult::WRONG_USER_PASS;

    // Check if password is correct.
    bool equal = false;
    if(!db->check_password(uid, password, equal))
        return CloseResult::IO_ERROR;
    if(!equal)
        return CloseResult::WRONG_USER_PASS;

    // Check if user is logged in.
    if(!db->user_logged_in(uid))
        return CloseResult::NOT_LOGGED_IN;

    // Check if the event exists.
    if(!db->event_exists(eid))
        return CloseResult::EVENT_DOES_NOT_EXIST;

    // Check if the event was created by UID.
    string creator;
    if(!db->get_event_creator(eid, creator))
        return CloseResult::IO_ERROR;
    if(creator != uid)
        return CloseResult::NOT_CREATED_BY_USER;
    
    // Check if event is sold out.
    bool sold_out = false;
    if(!db->is_event_sold_out(eid, sold_out))
        return CloseResult::IO_ERROR;
    if(sold_out)
        return CloseResult::SOLD_OUT;

    // Check if the event is on the past.
    bool in_past = false;
    if(!db->event_passed(eid, in_past))
        return CloseResult::IO_ERROR;
    if(in_past)
        return CloseResult::ALREADY_PASSED;

    // Check if the event was already closed.
    if(db->is_event_closed(eid))
        return CloseResult::ALREADY_CLOSED;
    
    
    if(!db->close_event(eid, DateTime::now()))
        return CloseResult::IO_ERROR;
    return CloseResult::SUCCESS;
}

ListResult list(vector<Event_list> &events){
    if(!db->get_all_events(events))
        return ListResult::IO_ERROR;

    if(events.empty())
        return ListResult::NO_EVENT_CREATED;
    return ListResult::SUCCESS;
}

ShowResult show(string &eid, Event_show_Info &data, string &filepath){
    if(!db->show_event(eid, data, filepath)){
        return ShowResult::ERROR;
    }
    return ShowResult::SUCCESS;
}

ReserveResult reserve(string &uid, string &password, string &eid, int &people, int &remaining_seats){
    // Check if user is logged in.
    if(!db->user_logged_in(uid))
        return ReserveResult::NOT_LOGGED_IN;

    // Check if password is correct.
    bool equal = false;
    if(!db->check_password(uid, password, equal))
        return ReserveResult::IO_ERROR;
    if(!equal)
        return ReserveResult::WRONG_PASS;

    // Check if event exists.
    if(!db->event_exists(eid))
        return ReserveResult::EVENT_NOT_ACTIVE;

    StartFileData data = db->extract_start_file_data(eid);
    int status = db->get_event_status(eid, data);

    switch(status){
        case EVENT_CORRUPTED:
            return ReserveResult::IO_ERROR;
        case EVENT_CLOSED:
            return ReserveResult::ALREADY_CLOSED;
        case EVENT_SOLD_OUT:
            return ReserveResult::SOLD_OUT;
        case EVENT_IN_PAST:
            return ReserveResult::EVENT_PASSED;
        case EVENT_ACCEPTING:
            if(!db->reserve(uid, eid, people, data, remaining_seats))
                return ReserveResult::IO_ERROR;
            if(remaining_seats > 0)
                return ReserveResult::TOO_MANY_SEATS;
            return ReserveResult::SUCCESS;
        default:
            return ReserveResult::IO_ERROR;
    }
}

ChangePassResult changePass(string &uid, string &old_password, string &new_password){
    // Check if user exists.
    if(!db->user_exists(uid))
        return ChangePassResult::NOT_REGISTERED;

    // Check if user is logged in.
    if(!db->user_logged_in(uid))
        return ChangePassResult::NOT_LOGGED_IN;

    // Check if password is correct.
    bool equal = false;
    if(!db->check_password(uid, old_password, equal))
        return ChangePassResult::IO_ERROR;
    if(!equal)
        return ChangePassResult::WRONG_PASS;

    if(!db->write_password(uid, new_password))
        return ChangePassResult::IO_ERROR;
    return ChangePassResult::SUCCESS;
}