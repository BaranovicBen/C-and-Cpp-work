#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Funkce je kamarad ne zradlo*/
void printAll(int low,int high, int print){
    int counter=0;
    int csqr;
    for (int a = low; a <= high;a++){
        for(int b = a; b<=high;b++ ){
            csqr=(a*a+b*b);
            int c=sqrt(csqr);
            if((c<=high) && (c*c == csqr) && c>b){
                if(print==1){
                    printf("* %d %d %d\n",a,b,c);
                }
                counter++; 
            }       
        }
    }
    printf("Celkem: %d\n",counter);
};

int main(){
    char action;
    int low,high,print;
    printf("%s\n","Problemy:");
    /*Ak scanf moze byt pouzite ako value int (pocet naskenovanych hodnot) a moze byt porovnany !=/== s cislom alebo s EOF!*/
    while (1){
        int vstup = scanf(" %c < %d ; %d >", &action, &low,&high);
        if(vstup==EOF){
            break;
        } else if (vstup != 3){
            printf("Nespravny vstup.\n");
            return 0;
        }
        
        if (low>high){
            printf("Nespravny vstup.\n");
            return 0;
        }
        /*Ak porovnavame char a znak tak znak musi byt v 'single uvodzovkach'*/
        if(action == '#'){
            print =0;
            printAll(low,high,print);
        } else if (action == '?'){ 
            print = 1;
            printAll(low,high,print);
        } else {
            printf("Nespravny vstup.\n");
        }
    }
    
    return 0;
}