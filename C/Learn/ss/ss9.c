#include <stdio.h>

int main(){
    int b = 0b0011;
    printf("Odd or even?\n");
    scanf("%d",&b);

    if (b & 1) {
        printf("%d is odd\n",b);
    } else {
        printf("%d is even\n",b);
    }

    return 0;
}