#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*Print out whole number from a decimal*/
int main(){
    char buffer[32];
    float input;
    int whole[2] = {0};

    printf("Zadaj cislo: ");
    scanf("%f",&input);


    snprintf(buffer, sizeof(buffer), "%f", input);

    char *token = strtok(buffer, ".");
    int i = 0;
    while (token != NULL && i < 2) {
        whole[i] = atoi(token); // Convert each part to an integer
        token = strtok(NULL, ".");
        i++;
    }
    printf("%d  . %d\n", whole[0], whole[1]);
    return 0;
}
