#include <stdio.h>

int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
 
    // return (arg1 > arg2) - (arg1 < arg2); // possible shortcut
 
    // return arg1 - arg2; // erroneous shortcut: undefined behavior in case of
                           // integer overflow, such as with INT_MIN here
}

int main(){
    int arr[10];
    int sum;

    printf("Gimme 10 nums\n");
    for(int i=0;i<10;i++){
        scanf("%d",&arr[i]);
        sum += arr[i];
        printf("Sum += %d\n", arr[i]);
    }

    int size = sizeof arr / sizeof *arr;
    qsort(arr, size, sizeof(int), compare_ints);

    float average = sum / 10;

    printf("Largest: %d, Smallest: %d, Average: %.2f\n",arr[9],arr[0],average);
 
    return 0;
}