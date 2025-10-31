#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DOW_MON      0b00000001
#define DOW_TUE      0b00000010
#define DOW_WED      0b00000100
#define DOW_THU      0b00001000
#define DOW_FRI      0b00010000
#define DOW_SAT      0b00100000
#define DOW_SUN      0b01000000
#define DOW_WORKDAYS (DOW_MON | DOW_TUE | DOW_WED | DOW_THU | DOW_FRI)
#define DOW_WEEKEND  (DOW_SAT | DOW_SUN)
#define DOW_ALL      (DOW_WORKDAYS | DOW_WEEKEND)

typedef struct TDate {
    unsigned m_Year;
    unsigned m_Month;
    unsigned m_Day;
} TDATE;

TDATE makeDate(unsigned y, unsigned m, unsigned d) {
    TDATE res = { y, m, d };
    return res;
}
#endif /* __PROGTEST__ */

int getDay(TDATE date); 
int get_century_anchor_day(unsigned year);
int get_year_doomsday(unsigned year, int century_anchor);
int get_month_doomsday(unsigned month, unsigned year);
long long int days_since_epoch(TDATE date);
long long int count_days_between(TDATE date1, TDATE date2);

#define YEAR_CACHE_SIZE 1000 

typedef struct {
    int year;
    int century_anchor;
    int year_doomsday;
} YearCacheEntry;

static YearCacheEntry yearCache[YEAR_CACHE_SIZE] = {0};

void computeYearDoomsdayCache(unsigned year, int *century_anchor, int *year_doomsday) {
    for (int i = 0; i < YEAR_CACHE_SIZE; i++) {
        if (yearCache[i].year == year) {
            *century_anchor = yearCache[i].century_anchor;
            *year_doomsday = yearCache[i].year_doomsday;
            return;
        }
    }

    *century_anchor = get_century_anchor_day(year);
    *year_doomsday = get_year_doomsday(year, *century_anchor);

    for (int i = 0; i < YEAR_CACHE_SIZE; i++) {
        if (yearCache[i].year == 0) {
            yearCache[i].year = year;
            yearCache[i].century_anchor = *century_anchor;
            yearCache[i].year_doomsday = *year_doomsday;
            break;
        }
    }
}

int getDay(TDATE date) {
    unsigned day = date.m_Day;
    unsigned month = date.m_Month;
    unsigned year = date.m_Year;

    int century_anchor, year_doomsday;
    computeYearDoomsdayCache(year, &century_anchor, &year_doomsday);

    int month_doomsday = get_month_doomsday(month, year);
    int day_difference = day - month_doomsday;
    day_difference = (day_difference % 7 + 7) % 7;
    int day_of_week = (year_doomsday + day_difference) % 7;

    return day_of_week;
}

int get_century_anchor_day(unsigned year) {
    static const int anchor_days[] = { 2, 0, 5, 3 };
    return anchor_days[(year / 100) % 4];
}

int get_year_doomsday(unsigned year, int century_anchor) {
    int y = year % 100;
    int a = y / 12;
    int b = y % 12;
    int c = b / 4;
    return (century_anchor + a + b + c) % 7;
}

int get_month_doomsday(unsigned month, unsigned year) {
    int is_leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    int doomsday_dates[] = {
        0,                    
        is_leap ? 4 : 3,      
        is_leap ? 29 : 28,   
        14,                  
        4,                   
        9,                   
        6,                  
        11,                  
        8,                 
        5,                    
        10,                
        7,                   
        12                    
    };
    return doomsday_dates[month];
}

int is_valid_date(TDATE date) {
    int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int is_leap = (date.m_Year % 4 == 0 && (date.m_Year % 100 != 0 || date.m_Year % 400 == 0)) && (date.m_Year != 4000);
    if (is_leap) {
        month_days[1] = 29;
    }
    if (date.m_Year < 2000 || date.m_Year > 4000) {
        return 0;
    }
    if (date.m_Month < 1 || date.m_Month > 12) {
        return 0; 
    }
    if (date.m_Day < 1 || date.m_Day > month_days[date.m_Month - 1]) {
        return 0; 
    }
    return 1;
}

long long int days_since_epoch(TDATE date) {
    long long int y = date.m_Year;
    long long int total_days = 365LL * (y - 1);
    long long int leap_days = (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400;
    total_days += leap_days;
    int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int is_leap = (date.m_Year % 4 == 0 && (date.m_Year % 100 != 0 || date.m_Year % 400 == 0));
    if (is_leap) {
        month_days[1] = 29;
    }
    for (int m = 0; m < date.m_Month - 1; m++) {
        total_days += month_days[m];
    }
    total_days += date.m_Day;
    return total_days;
}

long long int count_days_between(TDATE date1, TDATE date2) {
    return llabs(days_since_epoch(date2) - days_since_epoch(date1) + 1);
}

unsigned dayOfWeekMask(unsigned startDay, int dayOffset) {
    return (startDay + dayOffset) % 7;
}

long long countConnections(TDATE from, TDATE to, unsigned perWorkDay, unsigned dowMask) {
     if (!is_valid_date(from) || !is_valid_date(to)) {
        return -1; 
    }
    if (days_since_epoch(from) > days_since_epoch(to)) {
        return -1;
    }
    if (days_since_epoch(from) == days_since_epoch(to)) {
        unsigned singleDayOfWeek = getDay(from);
            if (dowMask == singleDayOfWeek) {
                return perWorkDay;
            }
        if (dowMask == DOW_WORKDAYS || dowMask == DOW_ALL || dowMask == DOW_WEEKEND) {
            if ((dowMask & DOW_WORKDAYS) && (singleDayOfWeek < 5)) { 
                return perWorkDay;
            } else if ((dowMask & DOW_WEEKEND) && (singleDayOfWeek >= 5)) { 
                if (singleDayOfWeek == 5) {
                    return (perWorkDay + 1) / 2;
                } else { 
                    return (perWorkDay + 2) / 3;
                }
            } else if (dowMask == DOW_ALL) { 
                return perWorkDay;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }
    long long totalDays = days_since_epoch(to) - days_since_epoch(from) + 1;
    long long totalConnections = 0;
    unsigned startDayOfWeek = getDay(from);
    for (long long i = 0; i < totalDays; i++) {
        unsigned currentDayOfWeek = dayOfWeekMask(startDayOfWeek, i);
        if (dowMask & (1 << currentDayOfWeek)) {
            if (currentDayOfWeek == 5) {
                totalConnections += (perWorkDay + 1) / 2;
            } else if (currentDayOfWeek == 6) {
                totalConnections += (perWorkDay + 2) / 3;
            } else {
                totalConnections += perWorkDay;
            }
        }
    }
    if (getDay(from) == getDay(to)) {
        totalConnections += 1;
        return totalConnections;
    } else {
        return totalConnections;
    }
}

TDATE date_from_days_since_epoch(long long days) {
    TDATE date;
    date.m_Year = 1;
    int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    while (days > 365) {
        int is_leap = (date.m_Year % 4 == 0 && (date.m_Year % 100 != 0 || date.m_Year % 400 == 0));
        if (is_leap) {
            days -= 366;
        } else {
            days -= 365;
        }
        date.m_Year++;
    }
    int is_leap = (date.m_Year % 4 == 0 && (date.m_Year % 100 != 0 || date.m_Year % 400 == 0));
    if (is_leap) {
        month_days[1] = 29;
    }
    date.m_Month = 1;
    for (int i = 0; i < 12; i++) {
        if (days <= month_days[i]) {
            break;
        }
        days -= month_days[i];
        date.m_Month++;
    }
    date.m_Day = days;
    return date;
}

TDATE endDate(TDATE from, long long connections, unsigned perWorkDay, unsigned dowMask) {
    if (!is_valid_date(from) || connections <= 0 || perWorkDay == 0 || dowMask == 0) {
        return makeDate(0, 0, 0);
    }
    unsigned startDayOfWeek = getDay(from);
    TDATE currentDate = from;
    long long connectionsInOneWeek = 0;
    if (dowMask == DOW_WORKDAYS) {
        connectionsInOneWeek = perWorkDay * 5;
    } else if (dowMask == DOW_WEEKEND) {
        connectionsInOneWeek = ((perWorkDay + 1) / 2) + ((perWorkDay + 2) / 3);
    } else if (dowMask == DOW_ALL) {
        connectionsInOneWeek = (perWorkDay * 5) + ((perWorkDay + 1) / 2) + ((perWorkDay + 2) / 3);
    } else {
        for (int i = 0; i < 7; i++) {
            if (dowMask & (1 << i)) {
                if (i == 6) {
                    connectionsInOneWeek += (perWorkDay + 1) / 2;
                } else if (i == 0) {
                    connectionsInOneWeek += (perWorkDay + 2) / 3;
                } else {
                    connectionsInOneWeek += perWorkDay;
                }
            }
        }
    }
    long long fullWeeks = connections / connectionsInOneWeek;
    long long remainingConnections = connections % connectionsInOneWeek;
    if (fullWeeks > 0) {
        long long daysToAdd = fullWeeks * 7;
        currentDate = date_from_days_since_epoch(days_since_epoch(currentDate) + daysToAdd);
        unsigned firstDayAfterFullWeeks = getDay(currentDate);
        if (dowMask & (1 << firstDayAfterFullWeeks)) {
            long long firstDayConnections;
            if (firstDayAfterFullWeeks == 6) {
                firstDayConnections = (perWorkDay + 1) / 2;
            } else if (firstDayAfterFullWeeks == 0) {
                firstDayConnections = (perWorkDay + 2) / 3;
            } else {
                firstDayConnections = perWorkDay;
            }
            if (remainingConnections < firstDayConnections) {
                currentDate = date_from_days_since_epoch(days_since_epoch(currentDate) - 1);
                return currentDate;
            }
        }
        if (__builtin_popcount(dowMask) == 1) {
            unsigned maskDay = __builtin_ctz(dowMask);
            unsigned currentDayOfWeek = getDay(currentDate);
            while (currentDayOfWeek != maskDay) {
                currentDate = date_from_days_since_epoch(days_since_epoch(currentDate) + 1);
                currentDayOfWeek = (currentDayOfWeek + 1) % 7;
            }
        }
    }
    while (remainingConnections > 0) {
        unsigned currentDayOfWeek = getDay(currentDate);
        if (dowMask & (1 << currentDayOfWeek)) {
            long long connectionsUsedToday;
            if (currentDayOfWeek == 6) {
                connectionsUsedToday = (perWorkDay + 1) / 2;
            } else if (currentDayOfWeek == 0) {
                connectionsUsedToday = (perWorkDay + 2) / 3;
            } else {
                connectionsUsedToday = perWorkDay;
            }
            if (remainingConnections < connectionsUsedToday) {
                return currentDate;
            }
            remainingConnections -= connectionsUsedToday;
            TDATE nextDate = date_from_days_since_epoch(days_since_epoch(currentDate) + 1);
            unsigned nextDayOfWeek = getDay(nextDate);
            if (dowMask & (1 << nextDayOfWeek)) {
                long long nextDayConnections;
                if (nextDayOfWeek == 6) {
                    nextDayConnections = (perWorkDay + 1) / 2;
                } else if (nextDayOfWeek == 0) {
                    nextDayConnections = (perWorkDay + 2) / 3;
                } else {
                    nextDayConnections = perWorkDay;
                }
                if (remainingConnections < nextDayConnections) {
                    return currentDate;
                }
            }
        }
        currentDate = date_from_days_since_epoch(days_since_epoch(currentDate) + 1);
    }
    return currentDate;
}

#ifndef __PROGTEST__
int main ()
{
  TDATE d;
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 31 ), 1, DOW_ALL ) == 31 );
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 31 ), 10, DOW_ALL ) == 266 );
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 31 ), 1, DOW_WED ) == 5 );
  assert ( countConnections ( makeDate ( 2024, 10, 2 ), makeDate ( 2024, 10, 30 ), 1, DOW_WED ) == 5 );
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 1 ), 10, DOW_TUE ) == 10 );
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 1 ), 10, DOW_WED ) == 0 );
  assert ( countConnections ( makeDate ( 2024, 1, 1 ), makeDate ( 2034, 12, 31 ), 5, DOW_MON | DOW_FRI | DOW_SAT ) == 7462 );
  assert ( countConnections ( makeDate ( 2024, 1, 1 ), makeDate ( 2034, 12, 31 ), 0, DOW_MON | DOW_FRI | DOW_SAT ) == 0 );
  assert ( countConnections ( makeDate ( 2024, 1, 1 ), makeDate ( 2034, 12, 31 ), 100, 0 ) == 0 );
  assert ( countConnections ( makeDate ( 2024, 10, 10 ), makeDate ( 2024, 10, 9 ), 1, DOW_MON ) == -1 );
  assert ( countConnections ( makeDate ( 2024, 2, 29 ), makeDate ( 2024, 2, 29 ), 1, DOW_ALL ) == 1 );
  assert ( countConnections ( makeDate ( 2023, 2, 29 ), makeDate ( 2023, 2, 29 ), 1, DOW_ALL ) == -1 );
  assert ( countConnections ( makeDate ( 2100, 2, 29 ), makeDate ( 2100, 2, 29 ), 1, DOW_ALL ) == -1 );
  assert ( countConnections ( makeDate ( 2400, 2, 29 ), makeDate ( 2400, 2, 29 ), 1, DOW_ALL ) == 1 );
  assert ( countConnections ( makeDate ( 4000, 2, 29 ), makeDate ( 4000, 2, 29 ), 1, DOW_ALL ) == -1 );

  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, DOW_ALL );
  assert ( d . m_Year == 2025 && d . m_Month == 1 && d . m_Day == 8 );

  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 6, DOW_ALL );
  assert ( d . m_Year == 2024 && d . m_Month == 10 && d . m_Day == 20 );

  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, DOW_WORKDAYS );
  assert ( d . m_Year == 2025 && d . m_Month == 2 && d . m_Day == 17 );

  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 4, DOW_WORKDAYS );
  assert ( d . m_Year == 2024 && d . m_Month == 11 && d . m_Day == 4 );

  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, DOW_THU );
  assert ( d . m_Year == 2026 && d . m_Month == 9 && d . m_Day == 2 );

  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 2, DOW_THU );
  assert ( d . m_Year == 2025 && d . m_Month == 9 && d . m_Day == 17 );

  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 0, DOW_THU );
  assert ( d . m_Year == 0 && d . m_Month == 0 && d . m_Day == 0 );

  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, 0 );
  assert ( d . m_Year == 0 && d . m_Month == 0 && d . m_Day == 0 );
    printf("PASSED ALL\n");
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */