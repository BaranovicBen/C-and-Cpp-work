#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 10000

static char input[MAX_LEN+1];
static char digits[MAX_LEN+1];
static int printAll = 0; 
static long long solCounter = 0; 
static const char *currentCodes[10000];
static int rnCounter = 0;
static int length;

static long long dp_odd[MAX_LEN+1]; 

static int compareNumbers(const char *a, const char *b) {
    while (*a == '0') a++;
    while (*b == '0') b++;
    int lenA = (int)strlen(a);
    int lenB = (int)strlen(b);
    if (lenA < lenB) return -1;
    if (lenA > lenB) return 1;
    int cmp = strcmp(a, b);
    return cmp;
}

static int isEven(const char *numberStr) {
    int len = (int)strlen(numberStr);
    char last = numberStr[len-1];
    int digit = last - '0';
    return (digit % 2 == 0);
}

static long long trackCountOnly(int pos, const char *lastCode, int lastEven) {
    if (pos == length) {
        return 1;
    }

    int useDpOdd = (!lastEven && lastCode == NULL);
    if (useDpOdd && dp_odd[pos] != -1) {
        return dp_odd[pos];
    }

    long long result = 0;
    for (int end = pos+1; end <= length; end++) {
        int codeLen = end - pos;
        char codeBuffer[MAX_LEN+1];
        memcpy(codeBuffer, digits+pos, codeLen);
        codeBuffer[codeLen] = '\0';

        int canUse = 1;
        if (lastCode && lastEven) {
            int cmp = compareNumbers(codeBuffer, lastCode);
            if (cmp < 0) canUse = 0;
        }

        if (canUse) {
            int theven = isEven(codeBuffer);
            long long ways = trackCountOnly(end, theven ? codeBuffer : NULL, theven);
            result += ways;
        }
    }

    if (useDpOdd) {
        dp_odd[pos] = result;
    }

    return result;
}

static void trackPrintSolutions(int pos, const char *lastCode, int lastEven) {
    if (pos == length) {
        solCounter++;
        if (printAll) {
            printf("* ");
            for (int i = 0; i < rnCounter; i++) {
                if (i > 0) printf(",");
                printf("%s", currentCodes[i]);
            }
            printf("\n");
        }
        return;
    }

    for (int end = pos+1; end <= length; end++) {
        int codeLen = end - pos;
        char codeBuffer[MAX_LEN+1];
        memcpy(codeBuffer, digits+pos, codeLen);
        codeBuffer[codeLen] = '\0';

        int canUse = 1;
        if (lastCode && lastEven) {
            int cmp = compareNumbers(codeBuffer, lastCode);
            if (cmp < 0) canUse = 0;
        }

        if (canUse) {
            int theven = isEven(codeBuffer);
            currentCodes[rnCounter++] = strdup(codeBuffer);
            trackPrintSolutions(end, theven ? currentCodes[rnCounter-1] : NULL, theven);
            free((char*)currentCodes[--rnCounter]);
        }
    }
}

int proInput(char *line, int *query, char *out) {
    char *start = line;
    while (isspace((unsigned char)*start)) start++;
    char *end = start + strlen(start) - 1;
    while (end >= start && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    if (*start != '?' && *start != '#') {
        return 0; 
    }

    *query = (*start == '?') ? 1 : 0;

    start++;
    while (isspace((unsigned char)*start)) start++;

    if (*start == '\0') {
        return 0;
    }
    char *ptr = start;
    while (*ptr && isdigit((unsigned char)*ptr)) ptr++;

    if (ptr == start) {
        return 0;
    }

    char *rest = ptr;
    while (isspace((unsigned char)*rest)) rest++;
    if (*rest != '\0') {
        return 0;
    }

    int dlen = (int)(ptr - start);
    memcpy(out, start, dlen);
    out[dlen] = '\0';

    return 1;
}

int main() {
    printf("Posloupnost:\n");

    while (1) {
        if (!fgets(input, sizeof(input), stdin)) break;
        if (strlen(input) == 0) continue;

        printAll = 0;
        solCounter = 0;
        rnCounter = 0;

        if (!proInput(input, &printAll, digits)) {
            printf("Nespravny vstup.\n");
            return 0;
        }

        length = (int)strlen(digits);
        for (int i = 0; i <= length; i++) {
            dp_odd[i] = -1;
        }

        long long totalWays = trackCountOnly(0, NULL, 0);
        if (printAll) {
            solCounter = 0;
            trackPrintSolutions(0, NULL, 0);
        } else {
            solCounter = totalWays;
        }

        printf("Celkem: %lld\n", solCounter);
    }
    return 0;
}