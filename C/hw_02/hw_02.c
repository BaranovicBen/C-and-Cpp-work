#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

void catchThemAll(int lo, int hi, int printify) {
    int count = 0;

    for (int a = lo; a <= hi; a++) {
        for (int b = a; b <= hi; b++) {
            int c_squared = a * a + b * b;
            int c = (int) sqrt(c_squared);

            if (c * c == c_squared && c <= hi && c > b) {
                if (printify) {
                    printf("* %d %d %d\n", a, b, c);
                }
                count++;
            }
        }
    }

    printf("Celkem: %d\n", count);
}

int main() {
    char quest;
    int lo, hi;

    printf("Problemy:\n");

    while ((quest = getchar()) != EOF) {
        if (isspace(quest)) continue;

        if (quest == '?' || quest == '#') {

            int ch = getchar();
            while (isspace(ch)) ch = getchar();

            if (ch != '<') {
                printf("Nespravny vstup.\n");
                return 0;
            }

            if (scanf(" %d ", &lo) != 1 || lo <= 0) {
                printf("Nespravny vstup.\n");
                return 0;
            }

            ch = getchar();
            if (ch != ';') {
                printf("Nespravny vstup.\n");
                return 0;
            }

            if (scanf(" %d ", &hi) != 1 || hi < lo) {
                printf("Nespravny vstup.\n");
                return 0;
            }

            ch = getchar();
            if (ch != '>') {
                printf("Nespravny vstup.\n");
                return 0;
            }

            if (quest == '?') {
                catchThemAll(lo, hi, 1);
            } else if (quest == '#') {
                catchThemAll(lo, hi, 0);
            }
        } else {
            printf("Nespravny vstup.\n");
            return 0;
        }

        getchar();
    }

    return 0;
}
