#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 1000000

void priceChange(int price, int arr[], int *size) {
    if (*size >= MAX) {
        printf("MOC.\n");
        return;
    }

    arr[*size] = price;
    (*size)++;
}

void maxProfLoss(int from, int to, int arr[]) {
    
    if (from > to || to - from < 1) {
        printf("Nejvyssi zisk: N/A\n");
        printf("Nejvyssi ztrata: N/A\n");
        return;
    }

    int maxProf = 0, profIdx1 = -1, profIdx2 = -1;
    int maxLoss = 0, lossIdx1 = -1, lossIdx2 = -1;

    int minPriceIdx = from;
    int maxPriceIdx = from;

    for (int i = from; i <= to; i++) {
        int profit = arr[i] - arr[minPriceIdx];
        if (profit > maxProf) {
            maxProf = profit;
            profIdx1 = minPriceIdx;
            profIdx2 = i;
        }

        if (arr[i] < arr[minPriceIdx]) {
            minPriceIdx = i;
        }

        int loss = arr[maxPriceIdx] - arr[i];
        if (loss > maxLoss) {
            maxLoss = loss;
            lossIdx1 = maxPriceIdx;
            lossIdx2 = i;
        }

        if (arr[i] > arr[maxPriceIdx]) {
            maxPriceIdx = i;
        }
    }

    if (profIdx1 != -1 && profIdx2 != -1) {
        printf("Nejvyssi zisk: %d (%d - %d)\n", maxProf, profIdx1, profIdx2);
    } else {
        printf("Nejvyssi zisk: N/A\n");
    }

    if (lossIdx1 != -1 && lossIdx2 != -1) {
        printf("Nejvyssi ztrata: %d (%d - %d)\n", maxLoss, lossIdx1, lossIdx2);
    } else {
        printf("Nejvyssi ztrata: N/A\n");
    }
}

int main() {
    char input[100];
    int from, to, price;
    int prices[MAX];
    int size = 0;

    printf("Ceny, hledani:\n");

    while (fgets(input, sizeof(input), stdin)) {

        if (input[0] == '+') {
            if (sscanf(input + 1, "%d", &price) != 1 || price <= 0) {
                printf("Nespravny vstup.\n");
                return 0;
            }
            priceChange(price, prices, &size);
        } else if (input[0] == '?') {
            if (sscanf(input + 1, "%d %d", &from, &to) != 2 || from < 0 || to >= size || from > to) {
                printf("Nespravny vstup.\n");
                return 0;
            }
            maxProfLoss(from, to, prices);
        } else {
            printf("Nespravny vstup.\n");
            return 0;
        }
    }

    return 0;
}
