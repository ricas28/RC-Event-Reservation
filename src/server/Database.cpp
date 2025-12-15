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
#include "../common/io.hpp"
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
        // File does not exist -> create it with 0
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

bool Database::increment_eid(int fd, int current_eid){
    int eid = 0;
    // Read current eid value if necessary.
    if(current_eid == -1){
        eid = read_eid(fd);
        eid++;
    }
    else{
        eid = current_eid + 1;
    }

    // Go back to the beginning of the file.
    lseek(fd, 0, SEEK_SET);
    if (ftruncate(fd, 0) == -1) {
        perror("ftruncate failed");
        close(fd);
        return false;
    }

    string eid_str =  to_string(eid);
    if(write_all(fd, eid_str.c_str(), eid_str.size()) <= 0){
        cerr << "failure to write to EID counter file" << endl;
        return false;
    }

    return true;
}

int Database::read_eid(int fd){
    char eid_str[EID_SIZE];

    if(read_all(fd, eid_str, EID_SIZE-1) <= 0){
        cerr << "Failure to read from EID counter file" << endl;
        return -1;
    }

    return atoi(eid_str);
}

bool Database::create_file(const string &path){
    int fd;
    if(!open_and_lock(path, O_CREAT | O_WRONLY, LOCK_EX, fd))
        return false;

    close(fd);  // Unlock
    return true;
}

bool Database::delete_file(const string &path){
    int fd;
    // Optional: try to lock before deleting
    if(open_and_lock(path, O_WRONLY, LOCK_EX, fd)){
        if(unlink(path.c_str()) == -1){
            if(errno != ENOENT){
                perror(("unlink failed for file " + path).c_str());
                close(fd);
                return false;
            }
        }
        close(fd);
    } else {
        // Could not lock -> maybe file doesn't exist
        if(errno != ENOENT){
            perror(("Failed to lock file before deleting: " + path).c_str());
            return false;
        }
    }
    return true;
}

bool Database::write_start_file(const string &start_path, const string &uid, Event_creation_Info &event){
    int fd;
    if(!open_and_lock(start_path, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, fd))
        return false;

    string content = uid + " " + event.name + " " + event.Fname + " " +
                          to_string(event.attendace_size) + " " +
                          event.event_date.toString() + "\n";

    if(write_all(fd, content.c_str(), content.size()) != (ssize_t)content.size()){
        perror(("write_all: " + start_path).c_str());
        delete_file(start_path);
        close(fd);
        return false;
    }

    close(fd); // unlock + close
    return true;
}

bool Database::write_res_file(const string &res_path, int reservations){
    int fd;
    if(!open_and_lock(res_path, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, fd))
        return false;

    string content = to_string(reservations) + "\n";

    if(write_all(fd, content.c_str(), content.size()) != (ssize_t)content.size()){
        perror(("write_all: " + res_path).c_str());
        delete_file(res_path);
        close(fd);
        return false;
    }

    close(fd); // unlock + close
    return true;
}

bool Database::write_description_file(const string &description_path, const string &Fdata){
    int fd;
    if(!open_and_lock(description_path, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, fd))
        return false;

    string content = Fdata;

    if(write_all(fd, content.c_str(), content.size()) != (ssize_t)content.size()){
        perror(("write_all: " + description_path).c_str());
        unlink(description_path.c_str());
        close(fd);
        return false;
    }

    close(fd); // unlock + close
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

bool Database::event_exists(const string &eid){
    return fs::exists(event_dir(eid));
}

bool Database::get_event_creator(const string &eid, string &creator){
    string start_path = event_start_file(eid);

    StartFileData data = extract_start_file_data(start_path);
    if(data.uid == -1)
        return false;

    creator = to_string(data.uid);
    return true;
}

bool Database::event_passed(const string &eid, bool &in_past){
    string start_file = event_start_file(eid);

    StartFileData data = extract_start_file_data(start_file);
    if(data.uid == -1)
        return false;

    in_past = data.start_date_time.isPast();
    return true;
}

bool Database::is_event_sold_out(const string &eid, bool &sold_out){
    string start_file = event_start_file(eid);
    string res_file = event_res_file(eid);

    StartFileData data = extract_start_file_data(start_file);
    if(data.uid == -1)
        return false;

    int reserved = get_reserved_seats(eid);
    if(reserved == -1)
        return false;

    sold_out = reserved >= data.event_attend;
    return true;
}

int Database::get_event_status(const string &eid, StartFileData data){
    // Check if event is closed.
    if(is_event_closed(eid)){
        return EVENT_CLOSED;
    }

    // Check for invalid values (extraction failed).
    if(data.uid == -1){
        // Push event corrupted.
        return EVENT_CORRUPTED;
    }

    // Check if event already occured.
    if(data.start_date_time.isPast()){
        // Close event
        close_event(eid, data.start_date_time);
        return EVENT_IN_PAST;
    }

    // Check if event is sold out or not.
    int reserved = get_reserved_seats(eid);
    if(reserved == -1){
        return EVENT_CORRUPTED;
    }
    int event_status = (reserved >= data.event_attend) ? EVENT_SOLD_OUT : EVENT_ACCEPTING;
    return event_status;
}

// ----------- OPERATIONS ---------------

bool Database::login_user(const string &uid){
    const string login_file = user_login_file(uid);
    return create_file(login_file);  // Already locks in create_file
}

bool Database::register_user(const string &uid, const string &password) {
    if (!ensure_user_dirs(uid)) return false;

    string pass_file = user_pass_file(uid);
    int fd;
    if(!open_and_lock(pass_file, O_CREAT | O_WRONLY | O_TRUNC, LOCK_EX, fd))
        return false;

    string data = password + "\n";
    if(write_all(fd, data.c_str(), data.size()) != (ssize_t)data.size()){
        perror(("Failed to write password to " + pass_file).c_str());
        close(fd);
        return false;
    }

    close(fd); // Release lock

    // Create login file
    if(!login_user(uid))
        return false;

    return true;
}

bool Database::check_password(const string &uid, const string &password){
    string pass_file = user_pass_file(uid);
    int fd;
    if(!open_and_lock(pass_file, O_RDONLY, LOCK_SH, fd))
        return false;

    char buf[PASS_SIZE];  
    ssize_t n = read_all(fd, buf, PASS_SIZE - 1);
    close(fd);

    if(n <= 0) return false;
    string stored(buf, (size_t)n);
    stored.erase(stored.find_last_not_of("\r\n")+1); // Remove newline

    return stored == password;
}

bool Database::logout_user(const string &uid){
    const string login_file = user_login_file(uid);
    int fd;
    if(open_and_lock(login_file, O_WRONLY, LOCK_EX, fd)){
        if(unlink(login_file.c_str()) == -1){
            if(errno != ENOENT){
                perror(("unlink for 'logout' failed: " + login_file).c_str());
                close(fd);
                return false;
            }
        }
        close(fd);
    }
    return true;   
}

bool Database::unregister_user(const string &uid){
    const string pass_file = user_pass_file(uid);
    int fd;

    if(open_and_lock(pass_file, O_WRONLY, LOCK_EX, fd)){
        if(unlink(pass_file.c_str()) == -1){
            if(errno != ENOENT){
                perror(("unlink failed for password file: " + pass_file).c_str());
                close(fd);
                return false;
            }
        }
        close(fd);
    }

    // Remove login file
    if(!logout_user(uid))
        return false;
    return true;
}

bool Database::close_event(const std::string &eid, DateTime dt){
    string end_file_path = event_end_file(eid);
    int fd;

    // Open and exclusively lock the file
    if(!open_and_lock(end_file_path, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, fd))
        return false;

    string data = dt.toString(true) + "\n";
    if(write_all(fd, data.c_str(), data.size()) != (ssize_t)data.size()){
        cerr << "Failed to write closing date to END file: " << end_file_path << endl;
        close(fd); // unlocks on close
        return false;
    }

    close(fd); // releases lock
    return true;
}

int Database::get_reserved_seats(const string &eid){
    string reservations_file = event_res_file(eid);
    int fd;

    // Open and shared lock for reading
    if(!open_and_lock(reservations_file, O_RDONLY, LOCK_SH, fd))
        return -1;

    char buf[64];
    ssize_t n = read_all(fd, buf, sizeof(buf)-1);
    close(fd); // releases lock

    if(n <= 0){
        cerr << "Failed to read reservations from file for event " << eid << endl;
        return -1;
    }

    int reservations = atoi(buf);
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

        StartFileData data = extract_start_file_data(event_start_file(eid));

        // Get status of events
        events.push_back({eid, get_event_status(eid, data)});
    }

    // Sort by EID.
    sort(events.begin(), events.end(),
     [](const pair<string, int> &a, const pair<string, int> &b) {
         return stoi(a.first) < stoi(b.first);
     });
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
    const string counter_path = base_path + EID_COUNTER;
    int fd;

    // Open and lock counter file for EID.
    if (!open_and_lock(counter_path, O_RDWR | O_CREAT, LOCK_EX, fd)) {
        return false;
    }

    int events_created = read_eid(fd);
    if(events_created == -1){
        close(fd);
        return false;
    }
    if(events_created == MAX_EVENTS){
        eid = "-1";
        close(fd);
        return false;
    }

    eid = format_eid(events_created + 1);
    if(!ensure_event_dirs(eid)){
        close(fd);
        return false;
    }
    string event_file = user_created_file(uid, eid);
    if(!create_file(event_file)){
        cerr << "Failure to open file on CREATED dir" << endl;
        close(fd);
        return false;
    } 

    string start_path = event_start_file(eid);
    string res_path = event_res_file(eid);
    string description_path = event_desc_file(eid, event.Fname);

    if(!write_start_file(start_path, uid, event)){
        delete_file(event_file);
        close(fd);
        return false;
    }

    if(!write_res_file(res_path, 0)){
        delete_file(event_file);
        delete_file(start_path);
        close(fd);
        return false;
    }

    if(!write_description_file(description_path, event.Fdata)){
        delete_file(event_file);
        delete_file(start_path);
        delete_file(res_path);
        close(fd);
        return false;
    }
    // Increase EID counter.
    if(!increment_eid(fd, events_created)){
        delete_file(event_file);
        delete_file(start_path);
        delete_file(res_path);
        close(fd);
        return false;
    }
    close(fd);
    return true;
 }

bool Database::get_all_events(vector<Event_list> &events){
    const string counter_path = base_path + EID_COUNTER;
    int fd;

    // Try to open and get a lock on the EID counter file.
    if (!open_and_lock(counter_path, O_RDWR | O_CREAT, LOCK_EX, fd)) {
        return false;
    }

    int current_events = read_eid(fd);
    close(fd);
    if(current_events == -1)
        return false;

    for (int i = 1; i <= current_events; i++){
        // Get EID.
        string eid = format_eid(i);

        StartFileData data = extract_start_file_data(event_start_file(eid));

        int status = get_event_status(eid, data);
        if(status == EVENT_CORRUPTED){
            events.push_back({eid, "-1", status, DateTime()});
        }
        else{
            events.push_back({eid, data.event_name, status, data.start_date_time});
        }
    }
    return true;
}
