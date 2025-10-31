// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// #define MAX_TRANSPORT_OPTIONS 100000
// #define MAX_PROBLEMS 1000
// #define ERROR_INVALID_INPUT "Nespravny vstup.\n"
// #define ERROR_CAPACITY "Prilis velky naklad, nelze odvezt.\n"

// typedef struct {
//     long long from;
//     long long to;
//     long long capacity;
//     long long cost;
// } TransportOption;

// typedef struct {
//     long long from;
//     long long quantity;
// } Problem;

// TransportOption transportOptions[MAX_TRANSPORT_OPTIONS];
// Problem problems[MAX_PROBLEMS];
// int transportOptionCount = 0;
// int problemCount = 0;

// int parseTransportOptions(const char *input);
// int getVal(const char *input);
// void calculateAndPrintResults();
// int calcCost(long long from, long long quantity, long long *finalDay, long long *totalExpns);

// const char *currentPos;

// long long readData() {
//     long long result = 0;
//     int sign = 1;
//     char ch;

//     while (isspace(ch = getchar()));

//     if (ch == '-') {
//         sign = -1;
//         ch = getchar();
//     }

//     if (!isdigit(ch)) {
//         return -1;
//     }

//     do {
//         result = result * 10 + (ch - '0');
//         ch = getchar();
//     } while (isdigit(ch));

//     ungetc(ch, stdin);
//     return sign * result;
// }

// int getVal(const char *input) {
//     long long from, quantity;
//     currentPos = input;

//     while (*currentPos) {
//         while (isspace(*currentPos)) currentPos++;

//         if (!isdigit(*currentPos) && *currentPos != '-' && *currentPos != '+') {
//             printf(ERROR_INVALID_INPUT);
//             return 0;
//         }

//         if (sscanf(currentPos, "%lld %lld", &from, &quantity) != 2) {
//             printf(ERROR_INVALID_INPUT);
//             return 0;
//         }

//         if (from < 0 || quantity <= 0) {
//             printf(ERROR_INVALID_INPUT);
//             return 0;
//         }

//         Problem problem;
//         problem.from = from;
//         problem.quantity = quantity;
//         problems[problemCount++] = problem;

//         while (*currentPos && *currentPos != '\n') currentPos++;
//         if (*currentPos == '\n') currentPos++;
//     }
//     return 1;
// }

// int calcCost(long long start_day, long long quantity, long long *final_day, long long *total_cost) {
//     long long remaining_quantity = quantity;
//     long long accumulated_cost = 0;

//     for (long long day = start_day; day <= 365; day++) {
//         long long daily_cost = 0;
//         long long daily_capacity = 0;

//         for (int j = 0; j < transportOptionCount; j++) {
//             TransportOption *option = &transportOptions[j];

//             if (day >= option->from && day <= option->to) {
//                 daily_cost += option->cost;
//                 daily_capacity += option->capacity;
//             }
//         }

//         accumulated_cost += daily_cost;

//         if (daily_capacity >= remaining_quantity) {
//             *final_day = day;
//             *total_cost = accumulated_cost;
//             return 1;
//         } else {
//             remaining_quantity -= daily_capacity;
//         }
//     }

//     return 0;
// }

// void printCost() {
//     long long finalDay = 0;
//     long long totalExpns = 0;

//     int i = problemCount - 1;

//     if (calcCost(problems[i].from, problems[i].quantity, &finalDay, &totalExpns)) {
//         printf("Konec: %lld, cena: %lld\n", finalDay, totalExpns);
//     } else {
//         printf(ERROR_CAPACITY);
//     }
// }

// int main() {
//     char ch;
//     transportOptionCount = 0;
//     problemCount = 0;

//     printf("Moznosti dopravy:\n");

//     ch = getchar();
//     while (isspace(ch)) {
//         ch = getchar();
//     }
//     if (ch != '{') {
//         printf(ERROR_INVALID_INPUT);
//         return 1;
//     }

//     while (1) {
//         while (isspace(ch = getchar()));

//         if (ch == '}') {
//             break;
//         }

//         if (ch != '[') {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         long long from = readData();
//         if (from < 0) {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         while (isspace(ch = getchar()));
//         if (ch != '-') {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         long long to = readData();
//         if (to < 0 || from > to) {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         while (isspace(ch = getchar()));
//         if (ch != ',') {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         long long capacity = readData();
//         if (capacity <= 0) {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         while (isspace(ch = getchar()));
//         if (ch != ',') {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         long long cost = readData();
//         if (cost <= 0) {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         while (isspace(ch = getchar()));
//         if (ch != ']') {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         if (transportOptionCount >= MAX_TRANSPORT_OPTIONS) {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }
        
//         TransportOption option;
//         option.from = from;
//         option.to = to;
//         option.capacity = capacity;
//         option.cost = cost;
//         transportOptions[transportOptionCount++] = option;

//         while (isspace(ch = getchar()));
//         if (ch == '}') {
//             break;
//         }
//         if (ch != ',') {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }
//     }

//     printf("Naklad:\n");
//     long long from, quantity;

//     while (1) {
//         int result = scanf("%lld %lld", &from, &quantity);

//         if (result != 2) {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         if (from < 0 || quantity <= 0) {
//             printf(ERROR_INVALID_INPUT);
//             return 1;
//         }

//         Problem problem;
//         problem.from = from;
//         problem.quantity = quantity;
//         problems[problemCount++] = problem;

//         printCost();
//     }
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRANSPORT_OPTIONS 100000
#define MAX_DAYS 1100 // Max days to handle (extra buffer beyond 365)
#define ERROR_INVALID_INPUT "Nespravny vstup.\n"
#define ERROR_CAPACITY "Prilis velky naklad, nelze odvezt.\n"

typedef struct {
    int from;
    int to;
    int capacity;
    int price;
} TransportOption;

TransportOption transportOptions[MAX_TRANSPORT_OPTIONS];
int transportOptionCount = 0;

// Event struct for capacity changes on specific days
typedef struct {
    int day;
    int capacityChange;
} Event;

Event events[MAX_TRANSPORT_OPTIONS * 2]; // Each transport option has a start and end event
int eventCount = 0;

// Function to compare events by day (for sorting)
int compareEvents(const void *a, const void *b) {
    return ((Event*)a)->day - ((Event*)b)->day;
}

int parseTransportOptions() {
    char ch;
    int from, to, capacity, price;

    // Parse input format validation
    if ((ch = getchar()) != '{') {
        printf(ERROR_INVALID_INPUT);
        return 0;
    }
    
    while ((ch = getchar()) != '}') {
        if (ch != '[') {
            printf(ERROR_INVALID_INPUT);
            return 0;
        }

        if (scanf("%d-%d, %d, %d", &from, &to, &capacity, &price) != 4) {
            printf(ERROR_INVALID_INPUT);
            return 0;
        }
        if (from < 0 || to < from || capacity <= 0 || price <= 0) {
            printf(ERROR_INVALID_INPUT);
            return 0;
        }

        if ((ch = getchar()) != ']') {
            printf(ERROR_INVALID_INPUT);
            return 0;
        }

        if (transportOptionCount >= MAX_TRANSPORT_OPTIONS) {
            printf(ERROR_INVALID_INPUT);
            return 0;
        }
        
        TransportOption option = {from, to, capacity, price};
        transportOptions[transportOptionCount++] = option;

        // Adding start and end events for each transport option
        events[eventCount++] = (Event){from, capacity};
        events[eventCount++] = (Event){to + 1, -capacity};

        // Check for end of transport list
        if ((ch = getchar()) == '}') break;
        if (ch != ',') {
            printf(ERROR_INVALID_INPUT);
            return 0;
        }
    }
    return 1;
}

int calcCost(int start_day, int quantity, int *final_day, long long *total_cost) {
    qsort(events, eventCount, sizeof(Event), compareEvents); // Sort events by day

    long long current_capacity = 0;
    long long accumulated_cost = 0;
    int remaining_quantity = quantity;
    int eventIndex = 0;
    
    for (int day = start_day; day < MAX_DAYS; day++) {
        // Update current capacity with events for the day
        while (eventIndex < eventCount && events[eventIndex].day == day) {
            current_capacity += events[eventIndex].capacityChange;
            eventIndex++;
        }
        
        // Calculate the transportable quantity and cost for the day
        if (current_capacity > 0) {
            int transport_today = (remaining_quantity < current_capacity) ? remaining_quantity : current_capacity;

            // Accumulate cost for today
            for (int i = 0; i < transportOptionCount; i++) {
                TransportOption *opt = &transportOptions[i];
                if (day >= opt->from && day <= opt->to) {
                    accumulated_cost += opt->price;
                }
            }

            remaining_quantity -= transport_today;
            if (remaining_quantity <= 0) {
                *final_day = day;
                *total_cost = accumulated_cost;
                return 1;
            }
        }
    }

    // If unable to transport the full quantity, return 0 for capacity error
    return 0;
}

void processRequests() {
    int start_day, quantity;
    int final_day;
    long long total_cost;

    printf("Naklad:\n");
    while (scanf("%d %d", &start_day, &quantity) == 2) {
        if (start_day < 0 || quantity <= 0) {
            printf(ERROR_INVALID_INPUT);
            return;
        }
        if (calcCost(start_day, quantity, &final_day, &total_cost)) {
            printf("Konec: %d, cena: %lld\n", final_day, total_cost);
        } else {
            printf(ERROR_CAPACITY);
        }
    }
}

int main() {
    printf("Moznosti dopravy:\n");
    if (!parseTransportOptions()) return 1;
    processRequests();
    return 0;
}
