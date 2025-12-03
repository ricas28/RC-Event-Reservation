#ifndef __DATETIME__HPP__
#define __DATETIME__HPP__

using namespace std;

class DateTime {
private:
    int day;
    int month;
    int year;
    int hour;
    int minute;

    // Returns how many days are on a month.
    int daysInMonth(int m, int y);

    // Checks if date is valid.
    bool isValidDate(int d, int m, int y);

    // Checks if time is valid.
    bool isValidTime(int h, int min);

public:
    // Constructor
    DateTime(int d = 1, int m = 1, int y = 2000,
             int h = 0, int min = 0);

    // Getters
    int getDay();
    int getMonth();
    int getYear();
    int getHour();
    int getMinute();

    // Setters
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);
    void setHour(int h);
    void setMinute(int min);

    // Methods

    // Checks if datetime is invalid
    bool invalidDateTime();

    // Advances the datetime by one minute
    void nextMinute();  

    // Prints the datetime (DD-MM-YYYY HH:MM)
    void print();  
    
    // Returns string representation (DD-MM-YYYY HH:MM)
    string toString();

    // Parse a Datetime object from a string
    static bool fromStrings(const string &date, const string &time, DateTime &out);

    // Compares this datetime to another (true if this > other)
    bool isAfter(DateTime& other);

    // Checks if datetime is in the past
    bool isPast();
};

#endif
