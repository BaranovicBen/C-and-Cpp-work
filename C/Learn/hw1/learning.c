#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>


#define MAX_INPUT_LENGTH 50
#define NUM_COUNT 4

double parse_exponential(const char *input) {
    char *e_ptr = strchr(input, 'e');
    if (!e_ptr) return -1;

    *e_ptr = '\0';
    double base = atof(input);  
    long exponent = atol(e_ptr + 1);
    return base * pow(10, exponent);
}

double parse_number(const char *input) {
    if (strchr(input, '-')) {
        printf("Nespravny vstup.\n");
        exit(0);
    }

    if (strchr(input, 'e')) {
        return parse_exponential(input);
    } else if (strchr(input, '.')) {
        return atof(input);
    } else {
        int value = atoi(input);
        if (value == 0 && strcmp(input, "0") != 0) {
            printf("Nespravny vstup.\n");
            exit(0);
        }
        return value;
    }
}

void read_rectangle_sides(double *numbers, int start_index) {
    char input[MAX_INPUT_LENGTH];
    printf("Obdelnik #%d:\n", (start_index / 2) + 1);

    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';
        char *token = strtok(input, " ");

        for (int i = 0; i < 2; i++) {
            if (!token) {
                printf("Nespravny vstup.\n");
                exit(0);
            }
            numbers[start_index + i] = parse_number(token);
            token = strtok(NULL, " ");
        }

        if (token) {
            printf("Nespravny vstup.\n");
            exit(0);
        }
    } else {
        printf("Nespravny vstup.\n");
        exit(0);
    }
}

int main() {
    double numbers[NUM_COUNT]; 

    read_rectangle_sides(numbers, 0);

    read_rectangle_sides(numbers, 2);

    double area1 = numbers[0] * numbers[1];
    double area2 = numbers[2] * numbers[3];

    if (fabs(area1 - area2) <  DBL_EPSILON * 1000) {
        printf("Obdelniky maji stejny obsah.\n");
    } else if (area1 > area2) {
        printf("Obdelnik #2 ma mensi obsah.\n");
    } else {
        printf("Obdelnik #1 ma mensi obsah.\n");
    }

    return 0;
}