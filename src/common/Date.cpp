#include <iostream>

#include "Date.hpp"

using namespace std;

// --- Helper functions ---
int Date::daysInMonth(int m, int y){
    switch (m) {
        case 2: // February
            return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 29 : 28;
        case 4: case 6: case 9: case 11:
            return 30;
        default:
            return 31;
    }
}

bool Date::isValidDate(int d, int m, int y){
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > daysInMonth(m, y)) return false;
    return true;
}


// --- Constructor ---
Date::Date(int d, int m, int y) {
    if (isValidDate(d, m, y)) {
        day = d;
        month = m;
        year = y;
    } else {
        // Default fallback date if invalid
        day = -1;
        month = -1;
        year = -1;
    }
}

// --- Getters ---
int Date::getDay() { return day; }
int Date::getMonth() { return month; }
int Date::getYear() { return year; }

// --- Setters ---
void Date::setDay(int d) {
    if (isValidDate(d, month, year)) day = d;
}

void Date::setMonth(int m) {
    if (isValidDate(day, m, year)) month = m;
}

void Date::setYear(int y) {
    if (isValidDate(day, month, y)) year = y;
}

// --- Methods ---

bool Date::invalidDate(){
    return day == -1 && month == -1 && year == -1;
}

void Date::nextDay() {
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

void Date::print() {
    cout << (day < 10 ? "0" : "") << day << "-"
              << (month < 10 ? "0" : "") << month << "-"
              << year;
}

bool Date::isAfter(Date& other) {
    if (year > other.year) return true;
    if (year < other.year) return false;

    if (month > other.month) return true;
    if (month < other.month) return false;

    return day > other.day;
}

bool Date::isPast() {
    // Get current date.
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    // tm_year is counting from 1900.
    Date today(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);

    // Date is past if isn't in the future or not the current date
    return !this->isAfter(today) && !(day == today.getDay() && month == today.getMonth() && year == today.getYear());
}

