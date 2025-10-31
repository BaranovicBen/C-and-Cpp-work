#include <stdio.h> //basic syntax
#include <string.h> //can use strlen and inout string
#include <ctype.h> //can use is digit
#include <stdlib.h> //can use atoi

char *quote[]={ //when making array of strings give 2 dimensions, one for num of string other for num of characters in each string. Or do empty brackets to pointers
    "What Is Broken Can Be Reforged.",
    "I'd rather make mistakes than make nothing at all.",
    "The darker the night, the brighter the stars.",
    "Beware the debts.",
    "The 'best' is wherever I decide to set the bar each day.",
    "Pain is temporary, victory is forever.",
    "A true master is an eternal student",
    "Mistakes only prove that you're trying!",
    "People fear what they cannot understand."
};

char *name[]={
    "Riven",
    "Ekko",
    "Braum",
    "Nautilus",
    "Draven",
    "Aatrox",
    "Master Yi",
    "Lux",
    "Syndra"
};


int main(){
    char input[100];
    int choice,lenght;
    printf("Cislo: ");
    scanf("%s", input);
    lenght = strlen(input);

    for(int i = 0; i < lenght; i++){
        if(!isdigit(input[i])){
            if(input[i] == '-'){
                choice = atoi(input);
                printf("Out of range %d\n",choice);
            } else {
               printf("Not a number\n");
            }

            return 0;
        }
    }

    choice = atoi(input);

    if(choice < 9){
        printf("\nQuote from %s\n",name[choice]);
        printf("%s\n", quote[choice]);
        printf("\n");
    } else {
        printf("Out of range %d\n",choice);
    }
    
    return 0;
}