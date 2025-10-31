#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>


float enumarate(char* given){
    float side;
    int num = 1;

    for (int i = 0; i<strlen(given);i++){
        if(!isdigit(given[i])){
            if(given[i]=='e'){
                num = 0;
                char* base = strtok(given, "e");
                char* exponent = strtok(NULL, "e");

                if(base && exponent){
                    float b = atof(base);
                    float exp = atof(exponent);

                    side = pow(b, exp);
                    return side;
                } else {
                    printf("Nespravny vstup\n");
                    return 0;
                }

            } else if (given[i] =='.'){
                int e = 0;
          
                for (int j = 0; j < strlen(given);j++){
                    if(given[j] =='e'){
                        e = 1;
                    }
                }

                if(e==1){
                    num = 0;
                    char* base = strtok(given, "e");
                    char* exponent = strtok(NULL, "e");

                    if(base && exponent){
                        float b = atof(base);
                        float exp = atof(exponent);

                        side = pow(b, exp);
                        return side;
                    } else {
                    printf("Nespravny vstup\n");
                    return 0;
                }
                } else {
                    side = atof(given);
                    num = 0;
                    return side;
                }

            } else {
                printf("Nespravny vstup.\n");
                return 0;
            }
        }
    }

    if(num==1){
        return atof(given);
    }
    return 0;
};

int main(){
    char height1[100], width1[100],height2[100], width2[100];
    printf("Obdelnik #1:\n");
    scanf("%s %s",height1, width1);
    float a1,b1,a2,b2,s1,s2;

    a1 = enumarate(height1);
    b1 = enumarate(width1);
    s1 = a1*b1;


    if(s1==0){
        return 0;
    }

    printf("Obdelnik #2:\n");
    scanf("%s %s",height2, width2);
    a2 = enumarate(height2);
    b2 = enumarate(width2);
    s2 = a2*b2;


    if(s2==0){
        return 0;
    }

    if(fabs(s1-s2) < DBL_EPSILON*1000){
        printf("Obdelniky maji stejny obsah.\n");
    } else if (s1>s2){
       printf("Obdelnik #2 ma mensi obsah.\n");
    } else {
        printf("Obdelnik #1 ma mensi obsah.\n");
    }
    
    return 0;
}