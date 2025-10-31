#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int pallindromer(char *list){
    int left = 0;
    int right = strlen(list) - 1;

    while (left < right) {
        // Skip non-alphanumeric characters and handle case insensitivity
        while (left < right && !isalnum(list[left])) left++;
        while (left < right && !isalnum(list[right])) right--;

        if (tolower((unsigned char)list[left]) != tolower((unsigned char)list[right])) {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
};

int main(){
    char *array= NULL;
    size_t capacity = 10;   

    printf("---Give palindrome---\n");

    if (getline(&array, &capacity, stdin) == -1) {
        printf("Error reading input or EOF encountered.\n");
        free(array);
        return 1;
    }

    printf("\n");

    if (array[strlen(array) - 1] == '\n') {
        array[strlen(array) - 1] = '\0';
    }

    if(pallindromer(array)){
        printf("pallindrome\n");
    } else {
        printf("not a pallindrome\n");
        return 0;
    }

    free(array);
    return 0;
}