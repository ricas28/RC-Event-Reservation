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
    int second;

    int daysInMonth(int m, int y);
    bool isValidDate(int d, int m, int y);
    bool isValidTime(int h, int min, int sec);

public:
    DateTime(int d = 1, int m = 1, int y = 2000,
             int h = 0, int min = 0, int sec = 0);

    int getDay();
    int getMonth();
    int getYear();
    int getHour();
    int getMinute();
    int getSecond();

    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);
    void setHour(int h);
    void setMinute(int min);
    void setSecond(int sec);

    bool invalidDateTime();

    void nextMinute();
    void nextSecond();

    void print(bool showSeconds= false);
    string toString(bool showSeconds= false);

    static bool fromStrings(const string &date, const string &time, DateTime &out);

    bool isAfter(DateTime& other);

    bool isPast();
};

#endif
