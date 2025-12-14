#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdlib.h>
#include <fcntl.h>           
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <cstring>   
#include <cerrno>

#include "../common/util.hpp"
#include "../common/DateTime.hpp"
#include "../common/protocol.hpp"
#include "../common/constants.hpp"
#include "server.hpp"
#include "Database.hpp"

using std::string;
namespace fs = std::filesystem;

#define DATABASE_FOLDER "ES_DB"
#define USERS_FOLDER "USERS"
#define EVENTS_FOLDER "EVENTS"
#define EID_COUNTER "eid_counter.txt"

Database::Database(): ok(true) {
    base_path   = "./" + string(DATABASE_FOLDER) + "/";
    users_path  = base_path + string(USERS_FOLDER) + "/";
    events_path = base_path + string(EVENTS_FOLDER) + "/";

    if(!safe_create_dir(users_path) || !safe_create_dir(events_path))
        ok = false;

    string counter_file = base_path + EID_COUNTER;
    ifstream infile(counter_file);
    if (!infile.is_open()) {
        // File does not exist → create it with 0
        ofstream outfile(counter_file);
        if (!outfile.is_open()) {
            cerr << "Failed to create counter file: " << counter_file << endl;
        } else {
            outfile << 0 << endl;
            outfile.close();
        }
    }
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
    return user_dir(uid) + uid + "_pass.txt";
}

string Database::user_login_file(const string& uid) const {
    return user_dir(uid) + uid + "_login.txt";
}

string Database::user_created_dir(const string& uid) const {
    return user_dir(uid) + "CREATED/";
}

string Database::user_created_file(const string &uid, const string &eid) const {
    return user_created_dir(uid) + eid + ".txt";
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
    return event_dir(eid) + "START_" + eid + ".txt";
}

string Database::event_res_file(const string& eid) const {
    return event_dir(eid) + "RES_" + eid + ".txt";
}

string Database::event_desc_dir(const string& eid) const {
    return event_dir(eid) + "DESCRIPTION/";
}

string Database::event_desc_file(const string& eid, const string& fname) const {
    return event_desc_dir(eid) + fname;
}

string Database::event_end_file(const string& eid) const {
    return event_dir(eid) + "END_" + eid + ".txt";
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

bool Database::increment_eid(){
    const string path = base_path + "eid_counter.txt";
    int eid = read_eid();

    ofstream out(path, ios::trunc);
    if (!out.is_open()) {
        cerr << "Failed to open EID counter file for writing" << endl;
        return false;
    }
    out << eid + 1;
    out.close();
    return true;
}

int Database::read_eid(){
    const string path = base_path + "/eid_counter.txt";
    int eid = 0;

    ifstream in(path);
    if (in.is_open()) {
        in >> eid;
        in.close();
    }
    return eid;
}

bool Database::create_file(const string &path){
    ofstream out(path);
    if (!out.is_open())
        return false;
    out.close();
    return true;
}

bool Database::delete_file(const string &path){
    if (unlink(path.c_str()) == -1) {
        if (errno != ENOENT) {     // ENOENT = file does not exist → OK
            fprintf(stderr,
                "unlink failed for file '%s': %s\n",
                path.c_str(),
                strerror(errno));
            return false;
        }
    }
    return true;
}

bool Database::write_start_file(const string &start_path, const string &uid, Event_creation_Info &event){
    ofstream start_fd(start_path);
    if (!start_fd.is_open()){
        cerr << "Failure to open START file" << endl;
        return false;
    }

    if(!(start_fd << uid << " " 
            << event.name << " " 
            << event.Fname << " "
            << event.attendace_size << " "
            << event.event_date.toString() << endl)){
        cerr << "Failure to write to START file" << endl;
        start_fd.close();
        delete_file(start_path);
        return false;
    }
    return true;
}

bool Database::write_res_file(const string &res_path, int reservations){
    ofstream file(res_path, ios::trunc);
    if (!file.is_open()) {
        cerr << "Failure to open RES file for writing: " << res_path << endl;
        return false;
    }

    if (!(file << reservations << endl)) {
        cerr << "Failure to write to RES file: " << res_path << endl;
        file.close();
        delete_file(res_path);
        return false;
    }

    file.close();
    return true;
}

bool Database::write_description_file(const string &description_path, string &Fdata){
    ofstream file(description_path, ios::trunc);
    if (!file.is_open()) {
        cerr << "Failure to open description file for writing: " << description_path << endl;
        return false;
    }

    if (!(file << Fdata << endl)) {
        cerr << "Failure to write to description file: " << description_path << endl;
        file.close();
        delete_file(description_path);
        return false;
    }

    file.close();
    return true;
}

bool Database::user_exists(const string &uid){
    return fs::exists(user_dir(uid));
}

bool Database::user_registered(const string &uid){
    return fs::exists(user_pass_file(uid));
}

bool Database::user_logged_in(const string &uid){
    return fs::exists(user_login_file(uid));
}

bool Database::is_event_closed(const string &eid){
    return fs::exists(event_end_file(eid));
}

bool Database::login_user(const string &uid){
    const string login_file = user_login_file(uid);
    return create_file(login_file);
}

bool Database::register_user(const string &uid, const string &password) {
    if (!ensure_user_dirs(uid)) return false;

    // Create password file.
    string pass_file = user_pass_file(uid);
    ofstream out(pass_file);
    if (!out.is_open())
        return false;
    out << password << "\n";

    // Create login file.
    if(!login_user(uid))
        return false;
   
    return true;
}

bool Database::check_password(const string &uid, const string &password){
    string pass_file = user_pass_file(uid);

    ifstream in(pass_file);
    if (!in.is_open()) return false;

    string stored;
    getline(in, stored);

    return stored == password;
}

bool Database::logout_user(const string &uid){
    if (unlink(user_login_file(uid).c_str()) == -1) {
        if (errno != ENOENT) {     // ENOENT = file does not exist → OK
            perror("unlink for 'logout' failed");
            return false;
        }
    }
    return true;   
}

bool Database::unregister_user(const string &uid){
    // Remove pass file.
    if (unlink(user_pass_file(uid).c_str()) == -1) {
        if (errno != ENOENT) {     // ENOENT = file does not exist → OK
            perror("unlink for 'logout' failed");
            return false;
        }
    }

    // Remove login file.
    if(!logout_user(uid))
        return false;
    
    return true;
}

bool Database::close_event(const std::string &eid, DateTime &dt){
    string end_file_path = event_end_file(eid);

    ofstream end_file(end_file_path);
    if (!end_file.is_open()) {
        cerr << "Failed to create END file for event " << eid << endl;
        return false;
    }

    // Write close date to file.
    if(!(end_file << dt.toString(true) << endl)){
        cerr << "Failute to write closing date to END file";
        end_file.close();
        return false;
    }

    end_file.close();
    return true;
}

int Database::get_reserved_seats(const string &eid){
    string reservations_file = event_res_file(eid);

    ifstream file(reservations_file);
    if (!file.is_open()) {
        cerr << "Failed to open reservations file for event " << eid << endl;
        return -1; 
    }

    int reservations = 0;
    if (!(file >> reservations)) {
        cerr << "Failed to read reservations from file for event " << eid << endl;
        return -1; 
    }

    return reservations;
}

void Database::get_user_events(const string &uid,  vector<pair<string, int>> &events){
    string user_events_path = user_created_dir(uid);

    // Iterate through files on the CREATED dir.
    for (const auto& entry : fs::directory_iterator(user_events_path)) {
        if (!entry.is_regular_file())
            continue;
        // Extract the eid of the event for each file.
        string eid = entry.path().stem().string();

        // Check if event is closed.
        if(is_event_closed(eid)){
            events.push_back({eid, EVENT_CLOSED});
            continue;
        }

        // Search for the START file.
        string start_file = event_start_file(eid);
        
        // Extract info from START file.
        StartFileData data = extract_start_file_data(start_file);
        // Check for invalid values (extraction failed).
        if(data.uid == -1){
            // Push event corrupted.
            events.push_back({eid, EVENT_CORRUPTED});
            continue;
        }

        // Check if event already occured.
        if(data.start_date_time.isPast()){
            // Close event
            close_event(eid, data.start_date_time);
            events.push_back({eid, EVENT_IN_PAST});
            continue;
        }

        // Check if event is sold out or not.
        int reserved = get_reserved_seats(eid);
        if(reserved == -1){
            events.push_back({eid, EVENT_CORRUPTED});
            continue;
        }
        int event_status = (reserved >= data.event_attend) ? EVENT_SOLD_OUT : EVENT_ACCEPTING;
        events.push_back({eid, event_status});
    }
}

void Database::get_user_reservations(const string &uid, vector<Reservation> &reservations){
    string reservations_path = user_reserved_dir(uid);

    vector<string> files;
    for (const auto &entry : fs::directory_iterator(reservations_path)) {
        if (!entry.is_regular_file())
            continue;
        files.push_back(entry.path().string());
    }

    // Sort by name.
    sort(files.begin(), files.end(), greater<string>());

    for (size_t i = 0; i < MAX_RESERVATIONS_SEND && i < files.size(); i++) {
        Reservation r = extract_reservation_file_data(files[i]);
        reservations.push_back(r);
    }
}

bool Database::create_event(const string &uid, Event_creation_Info &event, string &eid){
    int events_created = read_eid();
    if(events_created == MAX_EVENTS){
        eid = "-1";
        return false;
    }

    eid = format_eid(events_created + 1);
    if(!ensure_event_dirs(eid))
        return false;
    string event_file = user_created_file(uid, eid);
    if(!create_file(event_file)){
        cerr << "Failure to open file on CREATED dir" << endl;
        return false;
    } 

    string start_path = event_start_file(eid);
    string res_path = event_res_file(eid);
    string description_path = event_desc_file(eid, event.Fname);

    if(!write_start_file(start_path, uid, event)){
        delete_file(event_file);
        return false;
    }

    if(!write_res_file(res_path, 0)){
        delete_file(event_file);
        delete_file(start_path);
        return false;
    }

    if(!write_description_file(description_path, event.Fdata)){
        delete_file(event_file);
        delete_file(start_path);
        delete_file(res_path);
        return false;
    }
    // Increase EID counter.
    if(!increment_eid()){
        delete_file(event_file);
        delete_file(start_path);
        delete_file(res_path);
        return false;
    }
    return true;
 }
