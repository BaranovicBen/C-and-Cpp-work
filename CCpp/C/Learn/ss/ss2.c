#include <stdio.h>

int main(){
    int year;
    printf("Year:\n");
    scanf("%d",&year);
    /*Ak je rok delicelny 4 a zaroven nieje delitelny 100 alebo je delitelny 400 a zaroven nieje delitelny 4000*/
    if( (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) && (year % 4000 != 0)){
        printf("It is a leap year.\n");
    } else {
        printf("Not a leap year.\n");
    }
    return 0;
}