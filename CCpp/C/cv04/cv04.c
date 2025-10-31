#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void mulTable(int num) {
    int numSqrt = num * num;
    int w = 0;

    while (numSqrt > 0) {
        numSqrt /= 10;
        w++;
    }
    w++; 
    printf("%*s|", w - 1, "");
    for (int i = num; i >= 1; i--) {
        printf("%*d", w, i);
    }
    printf("\n");

    for (int i = 0; i < w - 1; i++) {
        printf("-");
    }
    printf("+");
    for (int i = num; i >= 1; i--) {
        for (int j = 0; j < w; j++) {
            printf("-");
        }
    }
    printf("\n");

    for (int i = 1; i <= num; i++) {
        printf("%*d|", w - 1, i);
        for (int j = num; j >= i; j--) {
            printf("%*d", w, i * j);
        }
        printf("\n");
    }
}

int main() {
    int num = 0;
    int valid = 1;
    int ch;

    printf("Rozsah:\n");

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (isdigit(ch)) {
            num = num * 10 + (ch - '0');
        } else {
            valid = 0;
            break;
        }
    }

    if (valid && num > 0) {
        mulTable(num);
    } else {
        printf("Nespravny vstup.\n");
    }

    return 0;
}
