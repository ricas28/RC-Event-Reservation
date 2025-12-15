#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

#include "DateTime.hpp"

using namespace std;

// --- Helper functions ---

int DateTime::daysInMonth(int m, int y){
    switch (m) {
        case 2:
            return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 29 : 28;
        case 4: case 6: case 9: case 11:
            return 30;
        default:
            return 31;
    }
}

bool DateTime::isValidDate(int d, int m, int y){
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > daysInMonth(m, y)) return false;
    return true;
}

bool DateTime::isValidTime(int h, int min, int sec){
    if (h < 0 || h > 23) return false;
    if (min < 0 || min > 59) return false;
    if (sec < 0 || sec > 59) return false;
    return true;
}


// --- Constructor ---

DateTime::DateTime(int d, int m, int y, int h, int min, int sec) {
    if (isValidDate(d, m, y) && isValidTime(h, min, sec)) {
        day = d;
        month = m;
        year = y;
        hour = h;
        minute = min;
        second = sec;
    } else {
        day = month = year = hour = minute = second = 0;
    }
}

// --- Getters ---

int DateTime::getDay() { return day; }
int DateTime::getMonth() { return month; }
int DateTime::getYear() { return year; }
int DateTime::getHour() { return hour; }
int DateTime::getMinute() { return minute; }
int DateTime::getSecond() { return second; }


// --- Setters ---

void DateTime::setDay(int d) {
    if (isValidDate(d, month, year)) day = d;
}

void DateTime::setMonth(int m) {
    if (isValidDate(day, m, year)) month = m;
}

void DateTime::setYear(int y) {
    if (isValidDate(day, month, y)) year = y;
}

void DateTime::setHour(int h) {
    if (isValidTime(h, minute, second)) hour = h;
}

void DateTime::setMinute(int min) {
    if (isValidTime(hour, min, second)) minute = min;
}

void DateTime::setSecond(int sec) {
    if (isValidTime(hour, minute, sec)) second = sec;
}


// --- Methods ---

bool DateTime::invalidDateTime(){
    return day == -1;
}

void DateTime::nextMinute() {
    nextSecond();
    // ignore detailed second handling here
}

void DateTime::nextSecond() {
    second++;
    if (second > 59) {
        second = 0;
        minute++;
        if (minute > 59) {
            minute = 0;
            hour++;
            if (hour > 23) {
                hour = 0;
                day++;
                if (day > daysInMonth(month, year)) {
                    day = 1;
                    month++;
                    if (month > 12) {
                        month = 1;
                        year++;
                    }
                }
            }
        }
    }
}

void DateTime::print(bool showSeconds){
    cout << toString(showSeconds) << endl;
}

string DateTime::DatetoString(bool inverted){
    ostringstream oss;
    if(inverted){
        oss << setfill('0')
            << setw(4) << year << "-"
            << setw(2) << month << "-"
            << setw(2) << day;
    }
    else{
        oss << setfill('0')
            << setw(2) << day << "-"
            << setw(2) << month << "-"
            << setw(4) << year;
    }
    return oss.str();
}

string DateTime::TimetoString(bool showSeconds){
    ostringstream oss;
    oss << setfill('0')
        << setw(2) << hour << ":"
        << setw(2) << minute;
    
    if(showSeconds)
       oss << ":" << setw(2) << second;
    return oss.str();
}

string DateTime::toString(bool showSeconds){
    return DatetoString() + " " + TimetoString(showSeconds);
}

bool DateTime::fromStrings(const string &date, const string &time, DateTime &out){
    int d, m, y, hh, mm, ss = 0;
    char dash1, dash2, colon1, colon2;

    // Parse dd-mm-yyyy
    istringstream date_ss(date);
    if (!(date_ss >> d >> dash1 >> m >> dash2 >> y) || dash1 != '-' || dash2 != '-'){
        cerr << "Invalid date format: " << date << endl;
        return false;
    }

    // Parse hh:mm OR hh:mm:ss
    istringstream time_ss(time);
    if (!(time_ss >> hh >> colon1 >> mm) || colon1 != ':'){
        cerr << "Invalid time format: " << time << endl;
        return false;
    }

    if (time_ss >> colon2 >> ss) {
        if (colon2 != ':') {
            cerr << "Invalid seconds format: " << time << endl;
            return false;
        }
    }

    DateTime dt(d, m, y, hh, mm, ss);
    if (dt.invalidDateTime()) {
        cerr << "Invalid datetime: " << date << " " << time << endl;
        return false;
    }

    out = dt;
    return true;
}

DateTime DateTime::now(){
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    return DateTime(
        now->tm_mday,
        now->tm_mon + 1,
        now->tm_year + 1900,
        now->tm_hour,
        now->tm_min,
        now->tm_sec
    );
}

bool DateTime::isAfter(DateTime& other) {
    if (year != other.year) return year > other.year;
    if (month != other.month) return month > other.month;
    if (day != other.day) return day > other.day;
    if (hour != other.hour) return hour > other.hour;
    if (minute != other.minute) return minute > other.minute;
    return second > other.second;
}

bool DateTime::isPast() {
    DateTime current = now();

    return !isAfter(current) &&
           !(
               day == current.getDay() &&
               month == current.getMonth() &&
               year == current.getYear() &&
               hour == current.getHour() &&
               minute == current.getMinute() &&
               second == current.getSecond()
           );
}
