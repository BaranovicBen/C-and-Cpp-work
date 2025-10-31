#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int X, Y;
    if (scanf("%d %d", &X, &Y) != 2 || X < 1 || Y < 1) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    // Dynamická alokácia matice
    int **mapa = (int **)malloc(X * sizeof(int *));
    int **prefix = (int **)malloc(X * sizeof(int *));
    for (int i = 0; i < X; i++) {
        mapa[i] = (int *)malloc(Y * sizeof(int));
        prefix[i] = (int *)malloc(Y * sizeof(int));
    }

    // Načítanie matice a výpočet prefixovej sumy
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            if (scanf("%d", &mapa[i][j]) != 1) {
                printf("Nespravny vstup.\n");
                return 1;
            }
            prefix[i][j] = mapa[i][j];
            if (i > 0) prefix[i][j] += prefix[i - 1][j];
            if (j > 0) prefix[i][j] += prefix[i][j - 1];
            if (i > 0 && j > 0) prefix[i][j] -= prefix[i - 1][j - 1];
        }
    }

    int size;
    while (scanf("%d", &size) == 1) {
        if (size < 1 || size > X || size > Y) {
            printf("Neexistuje.\n");
            continue;
        }

        int minCena = INT_MAX;

        // **Hľadanie všetkých štvorcov veľkosti `size × size`**
        for (int i = size - 1; i < X; i++) {
            for (int j = size - 1; j < Y; j++) {
                // Vypočítanie sumy štvorca pomocou prefixovej matice
                int sum = prefix[i][j];
                if (i - size >= 0) sum -= prefix[i - size][j];
                if (j - size >= 0) sum -= prefix[i][j - size];
                if (i - size >= 0 && j - size >= 0) sum += prefix[i - size][j - size];

                if (sum < minCena) {
                    minCena = sum;
                }
            }
        }

        // Ak neexistuje žiadny platný štvorec, vypíšeme "Neexistuje."
        if (minCena == INT_MAX) {
            printf("Neexistuje.\n");
            continue;
        }

        // Výpis všetkých možností s rovnakou minimálnou cenou
        printf("Cena: %d\n", minCena);
        for (int i = size - 1; i < X; i++) {
            for (int j = size - 1; j < Y; j++) {
                int sum = prefix[i][j];
                if (i - size >= 0) sum -= prefix[i - size][j];
                if (j - size >= 0) sum -= prefix[i][j - size];
                if (i - size >= 0 && j - size >= 0) sum += prefix[i - size][j - size];

                if (sum == minCena) {
                    printf("* (%d, %d)\n", i - size + 1, j - size + 1);
                }
            }
        }
    }

    // Uvoľnenie pamäte
    for (int i = 0; i < X; i++) {
        free(mapa[i]);
        free(prefix[i]);
    }
    free(mapa);
    free(prefix);

    return 0;
}
