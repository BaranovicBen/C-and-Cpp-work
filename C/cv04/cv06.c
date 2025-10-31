#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

int main() {
    double numA, numB;
    char action, equal_sign;

    printf("Zadejte vzorec:\n");

    int valid_input = scanf("%lf %c %lf %c", &numA, &action, &numB, &equal_sign);
    if (valid_input != 4 || equal_sign != '=') {
        printf("Nespravny vstup.\n");
        return 0;
    }

    switch (action) {
        case '+':
            printf("%.10g\n", numA + numB);
            break;
        case '-':
            printf("%.10g\n", numA - numB);
            break;
        case '*':
            {
                double result = numA * numB;
                if (result >= 1e10 || result <= -1e10 || (result <= 1e-10 && result >= -1e-10)) {
                    printf("%.1e\n", result);
                } else {
                    printf("%.10g\n", result);
                }
            }
            break;
        case '/':
            if (numB == 0) {
                printf("Nespravny vstup.\n");
            } else {
                double result = numA / numB;
                if (result >= INT_MAX || result <= -INT_MAX || (result <= 1e-10 && result >= -1e-10)) {
                    printf("%.1e\n", result);
                } else {
                    printf("%d\n", (int)result);
                }
            }
            break;
        default:
            printf("Nespravny vstup.\n");
            return 0;
    }

    return 0;
}
