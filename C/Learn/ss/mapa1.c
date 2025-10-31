#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Global Variables
int W, H;
int **mapa;  // DEI scores
int **dp;    // DP table for finding largest squares

// Function to allocate a 2D array dynamically
int** allocateMatrix(int rows, int cols) {
    int **matrix = (int **)malloc(rows * sizeof(int *));
    if (!matrix) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        if (!matrix[i]) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
    }
    return matrix;
}

// Function to free a 2D array
void freeMatrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to process a query
void processQuery(int minDEI) {
    if (minDEI < 0) {
        printf("Nespravny vstup.\n");
        exit(1);
    }

    // Create a binary matrix where 1 = valid, 0 = invalid
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            dp[i][j] = (mapa[i][j] >= minDEI) ? 1 : 0;
        }
    }

    int maxSize = 0;
    int **sizeMatrix = allocateMatrix(H, W);

    // Compute the largest square for each cell using DP
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (dp[i][j] == 1) {
                if (i == 0 || j == 0) {
                    sizeMatrix[i][j] = 1;
                } else {
                    sizeMatrix[i][j] = 1 + 
                        ((sizeMatrix[i-1][j] < sizeMatrix[i][j-1]) ?
                        ((sizeMatrix[i-1][j] < sizeMatrix[i-1][j-1]) ? sizeMatrix[i-1][j] : sizeMatrix[i-1][j-1]) :
                        ((sizeMatrix[i][j-1] < sizeMatrix[i-1][j-1]) ? sizeMatrix[i][j-1] : sizeMatrix[i-1][j-1]));
                }
                if (sizeMatrix[i][j] > maxSize) {
                    maxSize = sizeMatrix[i][j];
                }
            }
        }
    }

    if (maxSize == 0) {
        printf("Neexistuje.\n");
        freeMatrix(sizeMatrix, H);
        return;
    }

    printf("Nejvetsi: %dx%d\n", maxSize, maxSize);

    // Print all valid top-left corners of maxSize squares
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (sizeMatrix[i][j] == maxSize) {
                printf("* (%d,%d)\n", i - maxSize + 1, j - maxSize + 1);
            }
        }
    }

    freeMatrix(sizeMatrix, H);
}

int main() {
    // Read width and height
    if (scanf("%d %d", &W, &H) != 2 || W <= 0 || H <= 0) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    // Allocate memory for the map and DP table
    mapa = allocateMatrix(H, W);
    dp = allocateMatrix(H, W);

    // Read the DEI score map
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (scanf("%d", &mapa[i][j]) != 1 || mapa[i][j] < 0) {
                printf("Nespravny vstup.\n");
                return 1;
            }
        }
    }

    printf("Problemy:\n");

    int minDEI;
    while (1) {
        if(scanf("%d", &minDEI) !=1){
            printf("Nespravny vstup.\n");
            return 0;
        }
        processQuery(minDEI);
    }

    // Free allocated memory
    freeMatrix(mapa, H);
    freeMatrix(dp, H);

    return 0;
}
