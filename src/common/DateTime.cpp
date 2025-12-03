#include <iostream>
#include <sstream>
#include <ctime>

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

bool DateTime::isValidTime(int h, int min){
    if (h < 0 || h > 23) return false;
    if (min < 0 || min > 59) return false;
    return true;
}


// --- Constructor ---

DateTime::DateTime(int d, int m, int y, int h, int min) {
    if (isValidDate(d, m, y) && isValidTime(h, min)) {
        day = d;
        month = m;
        year = y;
        hour = h;
        minute = min;
    } else {
        day = month = year = hour = minute = -1;
    }
}


// --- Getters ---

int DateTime::getDay() { return day; }
int DateTime::getMonth() { return month; }
int DateTime::getYear() { return year; }
int DateTime::getHour() { return hour; }
int DateTime::getMinute() { return minute; }


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
    if (isValidTime(h, minute)) hour = h;
}

void DateTime::setMinute(int min) {
    if (isValidTime(hour, min)) minute = min;
}


// --- Methods ---

bool DateTime::invalidDateTime(){
    return day == -1;
}

void DateTime::nextMinute() {
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

void DateTime::print() {
    cout << (day < 10 ? "0" : "") << day << "-"
         << (month < 10 ? "0" : "") << month << "-"
         << year << " "
         << (hour < 10 ? "0" : "") << hour << ":"
         << (minute < 10 ? "0" : "") << minute << endl;
}

string DateTime::toString(){
    return (day < 10 ? "0" : "") + to_string(day) + "-" +
            (month < 10 ? "0" : "") + to_string(month) + "-" +
            to_string(year) + " " +
            (hour < 10 ? "0" : "") + to_string(hour) + ":" +
            (minute < 10 ? "0" : "") + to_string(minute);
}

bool DateTime::fromStrings(const string &date, const string &time, DateTime &out){
    int d, m, y, hh, mm;
    char dash1, dash2, colon1;

    // Parse dd-mm-yyyy
    istringstream date_ss(date);
    if (!(date_ss >> d >> dash1 >> m >> dash2 >> y) || dash1 != '-' || dash2 != '-'){
        cerr << "Invalid date format: " << date << endl;
        return false;
    }
    // Parse hh:mm
    istringstream time_ss(time);
    if (!(time_ss >> hh >> colon1 >> mm) || colon1 != ':'){
        cerr << "Invalid time format: " << time << endl;
        return false;
    }

    // Create DateTime e validate
    DateTime dt(d, m, y, hh, mm);
    if (dt.invalidDateTime()) {
        cerr << "Invalid datetime: " << date << " " << time << endl;
        return false;
    }
    // Successful parse.
    out = dt;
    return true;
}


bool DateTime::isAfter(DateTime& other) {
    if (year != other.year) return year > other.year;
    if (month != other.month) return month > other.month;
    if (day != other.day) return day > other.day;
    if (hour != other.hour) return hour > other.hour;
    return minute > other.minute;
}

bool DateTime::isPast() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    DateTime current(
        now->tm_mday,
        now->tm_mon + 1,
        now->tm_year + 1900,
        now->tm_hour,
        now->tm_min
    );

    return !isAfter(current) &&
           !(
              day == current.getDay() &&
              month == current.getMonth() &&
              year == current.getYear() &&
              hour == current.getHour() &&
              minute == current.getMinute()
           );
}
