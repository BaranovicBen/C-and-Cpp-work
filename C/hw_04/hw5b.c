#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_TRANSPORT_OPTIONS 100000
#define MAX_PROBLEMS 1000
#define MAX_DAYS 1035
#define ERROR_INVALID_INPUT "Nespravny vstup.\n"
#define ERROR_CAPACITY "Prilis velky naklad, nelze odvezt.\n"

typedef struct {
    long long from;
    long long to;
    long long capacity;
    long long cost;
} TransportOption;

typedef struct {
    long long from;
    long long quantity;
} Problem;

TransportOption transportOptions[MAX_TRANSPORT_OPTIONS];
Problem problems[MAX_PROBLEMS];
int transportOptionCount = 0;
int problemCount = 0;

long long dailyCapacity[MAX_DAYS] = {0};
long long dailyCost[MAX_DAYS] = {0};

void preprocessDays() {
    for (int i = 0; i < transportOptionCount; i++) {
        TransportOption *option = &transportOptions[i];
        for (long long day = option->from; day <= option->to; day++) {
            dailyCapacity[day] += option->capacity;
            dailyCost[day] += option->cost;
        }
    }
}

int calcCost(long long start_day, long long quantity, long long *finalDay, long long *totalCost) {
    *totalCost = 0;
    long long remains = quantity;

    for (long long day = start_day; day < MAX_DAYS; day++) {
        if (dailyCapacity[day] > 0) {
            long long trasnportedToday = (remains < dailyCapacity[day]) ? remains : dailyCapacity[day];
            *totalCost += dailyCost[day];
            remains -= trasnportedToday;

            if (remains <= 0) {
                *finalDay = day;
                return 1;
            }
        }
    }
    return 0; 
}

void printCost() {
    Problem *currentProblem = &problems[problemCount - 1];
    long long finalDay = 0;
    long long totalExpns = 0;

    if (calcCost(currentProblem->from, currentProblem->quantity, &finalDay, &totalExpns)) {
        printf("Konec: %lld, cena: %lld\n", finalDay, totalExpns);
    } else {
        printf(ERROR_CAPACITY);
    }
}

int main() {
    char ch;
    printf("Moznosti dopravy:\n");

    ch = getchar();
    while (isspace(ch)) ch = getchar();
    if (ch != '{') {
        printf(ERROR_INVALID_INPUT);
        return 1;
    }

    while (1) {s
        while (isspace(ch = getchar()));
        if (ch == '}') break;

        if (ch != '[') {
            printf(ERROR_INVALID_INPUT);
            return 1;
        }

        TransportOption option;
        if (scanf("%lld - %lld , %lld , %lld", &option.from, &option.to, &option.capacity, &option.cost) != 4 ||
            option.from < 0 || option.to < option.from || option.capacity <= 0 || option.cost <= 0) {
            printf(ERROR_INVALID_INPUT);
            return 1;
        }

        while (isspace(ch = getchar()));
        if (ch != ']') {
            printf(ERROR_INVALID_INPUT);
            return 1;
        }

        if (transportOptionCount >= MAX_TRANSPORT_OPTIONS) {
            printf(ERROR_INVALID_INPUT);
            return 1;
        }
        transportOptions[transportOptionCount++] = option;

        while (isspace(ch = getchar()));
        if (ch == '}') break;
        if (ch != ',') {
            printf(ERROR_INVALID_INPUT);
            return 1;
        }
    }

    preprocessDays();
    printf("Naklad:\n");
        long long from, quantity;

        while (1) {
            int result = scanf("%lld %lld", &from, &quantity);

            if (result == EOF) {
                break; 
            }

            if (result != 2) { 
                printf(ERROR_INVALID_INPUT);
                return 1;
            }

            if (from < 0 || quantity <= 0) {
                printf(ERROR_INVALID_INPUT);
                return 1;
            }

            Problem problem;
            problem.from = from;
            problem.quantity = quantity;
            problems[problemCount++] = problem;

            printCost();
        }
    return 0;
    return 0;
}