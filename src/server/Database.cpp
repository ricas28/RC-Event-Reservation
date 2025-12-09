#include <iostream>
#include <filesystem>

#include "../common/util.hpp"

#include "Database.hpp"

using std::string;
namespace fs = std::filesystem;

#define DATABASE_FOLDER "ES_DB"
#define USERS_FOLDER "USERS"
#define EVENTS_FOLDER "EVENTS"

Database::Database(): ok(true) {
    base_path   = "./" + string(DATABASE_FOLDER) + "/";
    users_path  = base_path + string(USERS_FOLDER) + "/";
    events_path = base_path + string(EVENTS_FOLDER) + "/";

    if(!safe_create_dir(users_path) || !safe_create_dir(events_path))
        ok = false;
}

Database& Database::instance() {
    static Database db;
    return db;
}

bool Database::is_ok() { return ok; }

// ----------------- USERS -----------------
string Database::user_dir(const string& uid) const {
    return users_path + uid + "/";
}

string Database::user_pass_file(const string& uid) const {
    return user_dir(uid) + uid + "-pass.txt";
}

string Database::user_login_file(const string& uid) const {
    return user_dir(uid) + uid + "-login.txt";
}

string Database::user_created_dir(const string& uid) const {
    return user_dir(uid) + "CREATED/";
}

string Database::user_reserved_dir(const string& uid) const {
    return user_dir(uid) + "RESERVED/";
}

// Ensure structure: USERS/<uid>/(pass/login/CREATED/RESERVED)
bool Database::ensure_user_dirs(const string& uid) {
    // Create USER dir.
    if (!safe_create_dir(user_dir(uid))) return false;

    // Create CREATED and RESERVED subdirs.
    if (!safe_create_dir(user_created_dir(uid)))  return false;
    if (!safe_create_dir(user_reserved_dir(uid))) return false;

    return true;
}

// ----------------- EVENTS -----------------
string Database::event_dir(const string& eid) const {
    return events_path + eid + "/";
}

string Database::event_start_file(const string& eid) const {
    return event_dir(eid) + "START" + eid + ".txt";
}

string Database::event_res_file(const string& eid) const {
    return event_dir(eid) + "RES" + eid + ".txt";
}

string Database::event_desc_dir(const string& eid) const {
    return event_dir(eid) + "DESCRIPTION/";
}

string Database::event_desc_file(const string& eid, const string& fname) const {
    return event_desc_dir(eid) + fname;
}

string Database::event_end_file(const string& eid) const {
    return event_dir(eid) + "END" + eid + ".txt";
}

string Database::event_reservations_dir(const string& eid) const {
    return event_dir(eid) + "RESERVATIONS/";
}

string Database::event_reservation_file(const string& eid,
                                        const string& filename) const {
    return event_reservations_dir(eid) + filename;
}

bool Database::ensure_event_dirs(const string& eid) {
    // Create EVENTS dir
    if (!safe_create_dir(event_dir(eid))) return false;

    // Create DESCRIPTION and RESERVATIONS subdirs.
    if (!safe_create_dir(event_desc_dir(eid)))          return false;
    if (!safe_create_dir(event_reservations_dir(eid)))  return false;

    return true;
}
