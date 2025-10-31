#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

int separator (char c){
    return c == '\0' || isspace((unsigned char)c) || ispunct((unsigned char)c);
}

 char * change (long given){
    char *result = (char *) calloc(64, sizeof(char));
    if (!result){
        return NULL;
    }
    result[0] = '\0';

    int t = given/1000;
    int s = (given % 1000)/100;
    int d = (given % 100)/10;
    int j = given % 10;

    for (int i = 0; i < t; i++){
        strcat(result, "M");
    }

    if (s == 9){
        strcat(result, "CM");
    } else if (s >= 5){
        strcat(result, "D");
        
        for (int i = 0; i < s - 5; i++){
            strcat(result, "C");
        }
    } else if (s == 4){
        strcat(result, "CD");
    } else {
        for (int i = 0; i < s; i++){
            strcat(result, "C");
        }
    }

    if (d == 9){
        strcat(result, "XC");
    } else if (d >= 5){
        strcat(result, "L");
        for (int i = 0; i < d - 5; i++){
            strcat(result, "X");
        }
    } else if (d == 4) {
        strcat(result, "XL");
    } else {
        for (int i = 0; i < d; i++){
            strcat(result, "X");
        }
    }

    if (j == 9) {
        strcat(result, "IX");
    } else if (j >= 5) {
        strcat(result, "V");
        for (int i = 0; i < j - 5; i++){
            strcat(result, "I");
        }
    } else if (j == 4) {
        strcat(result, "IV");
    } else {
        for (int i = 0; i < j; i++){
            strcat(result, "I");
        }
    }

    return result;
}

char * arabicToRoman ( const char * text ){
    char * result = NULL;
    size_t resLen = 0;
    size_t resMem = 0;

    const char * token = text;

    while (*token){
        if (isdigit((unsigned char)*token)){
            const char * startNum = token;
            while (isdigit((unsigned char)*token)){
                token++;
            }
            size_t numLen = token - startNum;

            char * fisrtNum = (char*) malloc(numLen + 1);
            if (!fisrtNum){
                if (result){
                    result[resLen] = '\0';
                }
                return result;
            }
            memcpy(fisrtNum, startNum, numLen);
            fisrtNum[numLen] = '\0';

            char before;
            if (startNum == text){
                before = '\0';
            } else{
                before = *(startNum - 1);
            }            
            char after = *token;
            int separated;
            if (separator(before) && separator(after)){
                separated = 1;
            } else {
                separated = 0;
            }

            long number = atol(fisrtNum);
            if (separated && number >= 1 && number <= 3999){
                char * roman = change(number);
                if (!roman) {
                    free(fisrtNum);
                    if (result){
                        result[resLen] = '\0';
                    }
                    return result;
                }

                size_t rLen = strlen(roman);
                while (resLen + rLen + 1 > resMem){
                    size_t newCap;
                    if (resMem == 0){
                        newCap = 64;
                    } else{
                        newCap = resMem * 2;
                    }

                    char * tmp = (char*) realloc(result, newCap);
                    if (!tmp){
                        free(fisrtNum);
                        free(roman);
                        if (result){
                            result[resLen] = '\0';
                        }
                        return result;
                    }
                    result = tmp;
                    resMem = newCap;
                }

                memcpy(result + resLen, roman, rLen + 1);
                resLen += rLen;
                free(roman);
            } else{
                while (resLen + numLen + 1 > resMem){
                    size_t newCap;
                    if (resMem == 0){
                        newCap = 64;
                    } else {
                        newCap = resMem * 2;
                    }

                    char * tmp = (char*) realloc(result, newCap);
                    if (!tmp){
                        free(fisrtNum);
                        if (result){
                            result[resLen] = '\0';
                        }
                        return result;
                    }
                    result = tmp;
                    resMem = newCap;
                }

                memcpy(result + resLen, fisrtNum, numLen + 1);
                resLen += numLen;
            }

            free(fisrtNum);
        } else{
            while (resLen + 2 > resMem){
                size_t newCap;
                if (resMem == 0){
                    newCap = 64;
                } else{
                    newCap = resMem * 2;
                }
                char * tmp = (char*) realloc(result, newCap);
                if (!tmp){
                    if (result){
                        result[resLen] = '\0';
                    }
                    return result;
                }
                result = tmp;
                resMem = newCap;
            }

            result[resLen++] = *token;
            result[resLen] = '\0';
            token++;
        }
    }

    if (!result){
        result = (char*) malloc(1);
        if (result){
            result[0] = '\0';
        }
    }

    return result;
}


#ifndef __PROGTEST__
int main (){
  char * r;

  r = arabicToRoman ( "CVUT FIT was founded on July 1-st 2009" );
  assert ( ! strcmp ( r, "CVUT FIT was founded on July I-st MMIX" ) );
  free ( r );

  r = arabicToRoman ( "PA1 is my favorite subject, rating 5 out of 5stars" );
  assert ( ! strcmp ( r, "PA1 is my favorite subject, rating V out of 5stars" ) );
  free ( r );

  r = arabicToRoman ( "The range of int data type is -2147483648 to 2147483647 inclusive." );
  assert ( ! strcmp ( r, "The range of int data type is -2147483648 to 2147483647 inclusive." ) );
  free ( r );

  r = arabicToRoman ( "There are 11 integers in closed internumber 10-20" );
  assert ( ! strcmp ( r, "There are XI integers in closed internumber X-XX" ) );
  free ( r );

  r = arabicToRoman ( "Chuck Norris and agent 007 are able to solve all Progtest homework problems on the 1-st try" );
  assert ( ! strcmp ( r, "Chuck Norris and agent VII are able to solve all Progtest homework problems on the I-st try" ) );
  free ( r );

  return EXIT_SUCCESS;
}
#endif  /* __PROGTEST__ */