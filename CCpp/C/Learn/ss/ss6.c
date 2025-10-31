#include <stdio.h>
#include <stdlib.h>

int main(){
    int *array = NULL;
    int capacity = 2;
    int size = 0;
    int input;

    /*vytvor mi list o velkosti x y*/
    array = (int *)malloc(capacity * sizeof(int));
    if (array == NULL){
        printf("No memory :(\n");
        return 0;
    }

    printf("-----Cisla-----\n");
    while (1){
        scanf(" %d",&input);

        if(input== -1){
            break;
        }

        if(size == capacity){
            capacity *= 2;
            int *new_array = (int *)realloc(array, capacity * sizeof(int));
            if(new_array == NULL){
                printf("No memory :(\n");
                free(array);
                return 0;
            }
            array = new_array;
        }

        array[size] = input;
        size++;
    }


    printf("-----Stored-----\n");
    for(int i=0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);
    return 0;
}