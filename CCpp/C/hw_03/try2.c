#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h> // Include for EXIT_SUCCESS

#define DOW_SUN      0b00000001
#define DOW_MON      0b00000010
#define DOW_TUE      0b00000100
#define DOW_WED      0b00001000
#define DOW_THU      0b00010000
#define DOW_FRI      0b00100000
#define DOW_SAT      0b01000000
#define DOW_WORKDAYS (DOW_MON | DOW_TUE | DOW_WED | DOW_THU | DOW_FRI)
#define DOW_WEEKEND  (DOW_SAT | DOW_SUN)
#define DOW_ALL      (DOW_WORKDAYS | DOW_WEEKEND)

typedef struct {
    int year;
    int month;
    int day;
} TDATE;

// Helper function to create a TDATE structure
TDATE makeDate(int year, int month, int day) {
    TDATE date = { year, month, day };
    return date;
}

bool validateDate(TDATE date);
int calculateConnectionsForDay(int dow, int perWorkDay);
void convertTDATEToStructTm(TDATE date, struct tm *tm_date);

// Function to count required connections over an interval
int countConnections(TDATE from, TDATE to, int perWorkDay, int dowMask) {
    if (!validateDate(from) || !validateDate(to) || perWorkDay <= 0 || dowMask == 0 ||
        (from.year > to.year || (from.year == to.year && (from.month > to.month || 
        (from.month == to.month && from.day > to.day))))) {
        return -1;
    }

    struct tm tm_date;
    convertTDATEToStructTm(from, &tm_date);

    int totalConnections = 0;
    while (tm_date.tm_year + 1900 < to.year || 
           (tm_date.tm_year + 1900 == to.year && (tm_date.tm_mon + 1 < to.month || 
            (tm_date.tm_mon + 1 == to.month && tm_date.tm_mday <= to.day)))) {

        int dow = tm_date.tm_wday;
        int dayMask = 1 << dow;
        if (dowMask & dayMask) {
            totalConnections += calculateConnectionsForDay(dow, perWorkDay);
        }

        tm_date.tm_mday++;
        mktime(&tm_date);  // Normalize date
    }
    return totalConnections;
}

// Function to calculate the last date covered by ordered connections
TDATE endDate(TDATE from, int connections, int perWorkDay, int dowMask) {
    if (!validateDate(from) || connections <= 0 || perWorkDay <= 0 || dowMask == 0) {
        return (TDATE){0, 0, 0}; // Invalid date
    }

    struct tm tm_date;
    convertTDATEToStructTm(from, &tm_date);

    int usedConnections = 0;
    while (usedConnections < connections) {
        int dow = tm_date.tm_wday;
        int dayMask = 1 << dow;
        if (dowMask & dayMask) {
            usedConnections += calculateConnectionsForDay(dow, perWorkDay);
        }

        if (usedConnections >= connections) break;
        tm_date.tm_mday++;
        mktime(&tm_date);  // Normalize date
    }

    TDATE result;
    result.year = tm_date.tm_year + 1900;
    result.month = tm_date.tm_mon + 1;
    result.day = tm_date.tm_mday;
    return result;
}

// Helper function to calculate connections based on the day of the week
int calculateConnectionsForDay(int dow, int perWorkDay) {
    if (dow >= 1 && dow <= 5) { // Weekday (Mon-Fri)
        return perWorkDay;
    } else if (dow == 6) {       // Saturday
        return (perWorkDay + 1) / 2;
    } else if (dow == 0) {       // Sunday
        return (perWorkDay + 2) / 3;
    }
    return 0;
}

// Helper function to validate if a date is correct
bool validateDate(TDATE date) {
    if (date.year < 2000 || date.month < 1 || date.month > 12 || date.day < 1) return false;
    struct tm tm_date = { .tm_year = date.year - 1900, .tm_mon = date.month - 1, .tm_mday = date.day };
    mktime(&tm_date);
    return (tm_date.tm_year + 1900 == date.year &&
            tm_date.tm_mon + 1 == date.month &&
            tm_date.tm_mday == date.day);
}

// Helper function to convert TDATE to struct tm
void convertTDATEToStructTm(TDATE date, struct tm *tm_date) {
    tm_date->tm_year = date.year - 1900;
    tm_date->tm_mon = date.month - 1;
    tm_date->tm_mday = date.day;
    tm_date->tm_hour = 0;
    tm_date->tm_min = 0;
    tm_date->tm_sec = 0;
    mktime(tm_date); // Normalize to set `tm_wday` properly
}

#ifndef __PROGTEST__
#include <assert.h>

int main() {
    TDATE d;
    d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_ALL);
    assert(d.year == 2025 && d.month == 1 && d.day == 8);

    int connections = countConnections(makeDate(2024, 10, 1), makeDate(2024, 12, 31), 6, DOW_WORKDAYS);
    assert(connections == 5 * 6 * 13); // Expected total connections for WORKDAYS in Oct-Dec with perWorkDay=6
    
    printf("All tests passed successfully.\n");
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
