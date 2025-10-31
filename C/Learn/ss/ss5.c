#include <stdio.h>

void swap(int *x, int *y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
};

int main(){
    int a,b;
    printf("Give nums\n");
    scanf("%d %d",&a,&b);

    printf("Before swap nums: %d %d\n",a,b);

    swap(&a,&b);

    printf("After swap nums: %d %d\n",a,b);

    return 0;
}