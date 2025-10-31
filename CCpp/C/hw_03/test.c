#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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


int getDayOfWeek(TDATE date);
int populate_active_days(unsigned dowMask, int *stack);
int isLeapYear(int year);
TDATE incrementDate(TDATE date);
long long countDaysBetween(TDATE from, TDATE to);
long long countConnections(TDATE from, TDATE to, unsigned perWorkDay, unsigned dowMask);
TDATE endDate(TDATE from, long long connections, unsigned perWorkDay, unsigned dowMask);

int getDayOfWeek(TDATE date) {
    int day = (int)date.m_Day;
    int month = (int)date.m_Month;
    int year = (int)date.m_Year;

    if (month < 3) {
        month += 12;
        year -= 1;
    }

    int k = day;
    int m = month;
    int D = year % 100;
    int C = year / 100;

    int f = k + (13 * (m + 1)) / 5 + D + D / 4 + C / 4 - 2 * C;
    int dayOfWeek = (f % 7 + 7) % 7;

    int adjustedDayOfWeek = (dayOfWeek + 6) % 7;

    return adjustedDayOfWeek;
}

int populate_active_days(unsigned dowMask, int *stack) {
    int stackTop = -1;
    const char *dayMapping[7] = {"1", "2", "3", "4", "5", "6", "0"};

    for (int i = 0; i < 7; i++) {
        if (dowMask & (1 << i)) {
            int adjustedDay = atoi(dayMapping[i]);
            stack[++stackTop] = adjustedDay;
        }
    }
    return stackTop + 1;
}

int isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) && (year % 4000 != 0);
}

TDATE incrementDate(TDATE date) {
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int daysThisMonth = daysInMonth[date.m_Month - 1];    
    if (date.m_Month == 2 && isLeapYear((int)date.m_Year)) {
        daysThisMonth = 29;
    }

    date.m_Day++;
    if (date.m_Day > (unsigned)daysThisMonth) { 
        date.m_Day = 1;
        date.m_Month++;
    }

    if (date.m_Month > 12) {
        date.m_Month = 1;
        date.m_Year++;
    }

    return date;
}

long long countDaysBetween(TDATE from, TDATE to) {
    if (from.m_Year < 2000 || to.m_Year < 2000) {
        return -1;
    }

    if (from.m_Month < 1 || from.m_Month > 12 || to.m_Month < 1 || to.m_Month > 12) {
        return -1;
    }

    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int daysThisMonthFrom = daysInMonth[from.m_Month - 1];
    int daysThisMonthTo = daysInMonth[to.m_Month - 1];

    if (from.m_Month == 2 && isLeapYear((int)from.m_Year)) daysThisMonthFrom = 29;
    if (to.m_Month == 2 && isLeapYear((int)to.m_Year)) daysThisMonthTo = 29;

    if (from.m_Day < 1 || from.m_Day > (unsigned)daysThisMonthFrom) {
        return -1;
    }
    if (to.m_Day < 1 || to.m_Day > (unsigned)daysThisMonthTo) { 
        return -1;
    }

    long long dayCount = 0;
    int year = from.m_Year, month = from.m_Month, day = from.m_Day;

    while ((unsigned)year < to.m_Year || ((unsigned)year == to.m_Year && ((unsigned)month < to.m_Month || ((unsigned)month == to.m_Month && (unsigned)day <= to.m_Day)))) {
        dayCount++;
        day++;

        int daysThisMonth = daysInMonth[month - 1];
        if (month == 2 && isLeapYear(year)) daysThisMonth = 29;

        if (day > daysThisMonth) {
            day = 1;
            month++;
        }
        if (month > 12) {
            month = 1;
            year++;
        }
    }

    if (dayCount < 1) {
        return -1;
    }

    return dayCount;
}

long long countConnections(TDATE from, TDATE to, unsigned perWorkDay, unsigned dowMask) {
    long long daysBetween = countDaysBetween(from, to);
    if (daysBetween < 1) return -1;

    int startDow = getDayOfWeek(from);

    int activeDaysStack[7] = {1,2,3,4,5,6,0};
    int activeDaysCount = populate_active_days(dowMask, activeDaysStack); 

    long long connections = 0;

    int day = startDow;
    printf("Active days stack: {");
    for (int i = 0; i < activeDaysCount; i++) {
        printf("%d ", activeDaysStack[i]);
    }
    printf("}\n");

    printf("Entering initial while loop to calculate partial week connections\n");
    while (day != 0) {  
        int isActiveDay = 0;
        for (int i = 0; i < activeDaysCount; i++) {  
            if (activeDaysStack[i] == day) {
                isActiveDay = 1;
                printf("Debug: Found active day %d in activeDaysStack\n", day);
                break;
            }
        }

        if (isActiveDay) {
            if (day >= 1 && day <= 5) {
                connections += perWorkDay;
                printf("Debug: Added weekday connections, new connections total = %lld\n", connections);
            } else if (day == 6) { 
                connections += (perWorkDay + 1) / 2;
                printf("Debug: Added Saturday connections, new connections total = %lld\n", connections);
            } else if (day == 0) { 
                connections += (perWorkDay + 2) / 3;
                printf("Debug: Added Sunday connections, new connections total = %lld\n", connections);
            }
        } else {
            printf("Debug: Day %d is NOT active, skipping\n", day);
        }

        daysBetween--;
        if (daysBetween == 0) {
            printf("Debug: No more days remaining, exiting loop with connections = %lld\n", connections);
            return connections;
        }
        
        day = (day + 1) % 7; 
    }
    printf("Exiting initial while loop. Connections after partial week = %lld, Remaining daysBetween = %lld\n", connections, daysBetween);
    int weeklyConnections = 0;
    for (int i = 0; i < activeDaysCount; i++) { 
        int dayInWeek = activeDaysStack[i];
        if (dayInWeek >= 1 && dayInWeek <= 5) {
            weeklyConnections += perWorkDay;
        } else if (dayInWeek == 6) {
            weeklyConnections += (perWorkDay + 1) / 2;
        } else if (dayInWeek == 0) {
            weeklyConnections += (perWorkDay + 2) / 3;
        }
    }
    printf("Calculated weekly connections = %d\n", weeklyConnections);

    long long fullWeeks = daysBetween / 7;
    connections += fullWeeks * weeklyConnections;
    daysBetween -= fullWeeks * 7;
    printf("After full weeks calculation: Connections = %lld, Remaining daysBetween = %lld\n", connections, daysBetween);

    day = 0;
    printf("Entering final while loop to calculate remaining days\n");
    while (daysBetween > 0) {
        int isActiveDay = 0;
        for (int i = 0; i < activeDaysCount; i++) { 
            if (activeDaysStack[i] == day) {
                isActiveDay = 1;
                break;
            }
        }

        if (isActiveDay) {
            if (day >= 1 && day <= 5) {
                connections += perWorkDay;
                printf("Debug: Added weekday connections, new connections total = %lld\n", connections);
            } else if (day == 6) {
                connections += (perWorkDay + 1) / 2;
                printf("Debug: Added Saturday connections, new connections total = %lld\n", connections);
            } else if (day == 0) {
                connections += (perWorkDay + 2) / 3;
                printf("Debug: Added Sunday connections, new connections total = %lld\n", connections);
            }
        } else {
            printf("Debug: Day %d is NOT active, skipping\n", day);
        }

        day = (day + 1) % 7;
        daysBetween--;
    }
    printf("Exiting final while loop. Total connections = %lld\n", connections);
    printf("\n");
    printf("\n");


    return connections;
}
TDATE endDate(TDATE from, long long connections, unsigned perWorkDay, unsigned dowMask) {
    if (from.m_Year < 2000 || from.m_Month < 1 || from.m_Month > 12 || from.m_Day < 1 || from.m_Day > 31 ||
        connections < 0 || perWorkDay == 0 || dowMask == 0) {
        return makeDate(0, 0, 0); 
    }

    TDATE currentDate = from;
    int activeDaysStack[7] = {1, 2, 3, 4, 5, 6, 0};
    int activeDaysCount = populate_active_days(dowMask, activeDaysStack);

    if (activeDaysCount == 0 || connections < perWorkDay) {
        return makeDate(0, 0, 0); 
    }

    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    while (connections > 0) {
        int dayOfWeek = getDayOfWeek(currentDate);
        int isActiveDay = 0;

        for (int i = 0; i < activeDaysCount; i++) {
            if (activeDaysStack[i] == dayOfWeek) {
                isActiveDay = 1;
                break;
            }
        }

        if (isActiveDay) {
            int dailyFrequency = 0;
            if (dayOfWeek >= 1 && dayOfWeek <= 5) {
                dailyFrequency = perWorkDay;
            } else if (dayOfWeek == 6) {
                dailyFrequency = (int)ceil(perWorkDay / 2.0);
            } else if (dayOfWeek == 0) {
                dailyFrequency = (int)ceil(perWorkDay / 3.0);
            }

            if (connections < dailyFrequency) {
                if (currentDate.m_Day > 1) {
                    currentDate.m_Day -= 1;
                } else {
                    if (currentDate.m_Month > 1) {
                        currentDate.m_Month -= 1;
                        currentDate.m_Day = daysInMonth[currentDate.m_Month - 1];
                        if (currentDate.m_Month == 2 && isLeapYear((int)currentDate.m_Year)) {
                            currentDate.m_Day = 29;
                        }
                    } else {
                        currentDate.m_Year -= 1;
                        currentDate.m_Month = 12;
                        currentDate.m_Day = 31;
                    }
                }
                return currentDate;
            }

            connections -= dailyFrequency;
        }

        do {
            currentDate = incrementDate(currentDate);
            dayOfWeek = getDayOfWeek(currentDate);
            isActiveDay = 0;
            for (int i = 0; i < activeDaysCount; i++) {
                if (activeDaysStack[i] == dayOfWeek) {
                    isActiveDay = 1;
                    break;
                }
            }
        } while (!isActiveDay);

        if (connections == 0) {
            if (currentDate.m_Day > 1) {
                currentDate.m_Day -= 1;
            } else {
                if (currentDate.m_Month > 1) {
                    currentDate.m_Month -= 1;
                    currentDate.m_Day = daysInMonth[currentDate.m_Month - 1];
                    if (currentDate.m_Month == 2 && isLeapYear((int)currentDate.m_Year)) {
                        currentDate.m_Day = 29;
                    }
                } else {
                    currentDate.m_Year -= 1;
                    currentDate.m_Month = 12;
                    currentDate.m_Day = 31;
                }
            }
            return currentDate;
        }
    }

    return currentDate;
}

void runTests() {
    FILE *file = fopen("testEndDateFinal.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    int passCount = 0;
    int failCount = 0;
    char line[256];

    TDATE from;
    unsigned perWorkDay = 0, dowMask = 0;
    unsigned y1, m1, d1, y2, m2, d2;
    long long connections;
    int expectedYear, expectedMonth, expectedDay;

    while (fgets(line, sizeof(line), file)) {
        // Attempt to parse as `endDate` line
        int numParsed = sscanf(line, "d = endDate ( makeDate ( %u , %u , %u ), %lld , %u , %x );", &y1, &m1, &d1, &connections, &perWorkDay, &dowMask);
        
        if (numParsed == 6) {
            from = makeDate(y1, m1, d1);
            continue;
        }

        // Attempt to parse as `assert` line
        numParsed = sscanf(line, "assert ( (d . m_Year == %d && d . m_Month == %d && d . m_Day == %d) );", &expectedYear, &expectedMonth, &expectedDay);
        
        if (numParsed == 3) {
            TDATE result = endDate(from, connections, perWorkDay, dowMask);

            if (result.m_Year == expectedYear && result.m_Month == expectedMonth && result.m_Day == expectedDay) {
                passCount++;
            } else {
                printf("Test failed for from date (%u-%u-%u), connections: %lld, perWorkDay: %u, dowMask: %x. Expected %d-%d-%d, got %u-%u-%u\n",
                       y1, m1, d1, connections, perWorkDay, dowMask, expectedYear, expectedMonth, expectedDay,
                       result.m_Year, result.m_Month, result.m_Day);
                failCount++;
            }
            continue;
        }

        printf("Warning: Line could not be parsed correctly:\n%s\n", line);
    }    fclose(file);
    printf("Tests completed: %d passed, %d failed\n", passCount, failCount);
}

void runTests2() {
    FILE *file = fopen("finalTests.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    int passCount = 0;
    int failCount = 0;
    char line[256];

    unsigned y1, m1, d1, y2, m2, d2, perWorkDay, dowMask;
    long long expectedResult;

    while (fgets(line, sizeof(line), file)) {
        int numParsed = sscanf(line, "assert ( countConnections ( makeDate ( %u, %u, %u ), makeDate ( %u, %u, %u ), %u, %x ) == %lld );",
                               &y1, &m1, &d1, &y2, &m2, &d2, &perWorkDay, &dowMask, &expectedResult);
        
        if (numParsed != 9) {
            printf("Warning: Line could not be parsed correctly:\n%s\n", line);
            continue; 
        }

        TDATE from = makeDate(y1, m1, d1);
        TDATE to = makeDate(y2, m2, d2);
        long long result = countConnections(from, to, perWorkDay, dowMask);

        if (result == expectedResult) {
            passCount++;
        } else {
            printf("Test failed for (%u-%u-%u to %u-%u-%u, perWorkDay: %u, dowMask: %x). Expected %lld, got %lld\n",
                   y1, m1, d1, y2, m2, d2, perWorkDay, dowMask, expectedResult, result);
            failCount++;
        }
    }

    fclose(file);

    printf("Tests completed: %d passed, %d failed\n", passCount, failCount);
}

#ifndef __PROGTEST__
int main() {    
    TDATE d;
    runTests();
    runTests2();

    printf("PASSED ALL\n");
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */