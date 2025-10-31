#include <stdio.h>
#include <stdlib.h>

int main(){
    int sum = 0;
    char *buffer=NULL;
    int lineCounter = 0;
    size_t bufferSize = 10;

    FILE* fp = fopen("ss10.txt","r");
    if (!fp){
        perror("File opening failed");
        return 0;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            lineCounter++;
        }
    }

    rewind(fp);


    buffer = (char *)malloc(bufferSize * sizeof(char));
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < lineCounter; i++) {
        if (fscanf(fp, "%s", buffer) == 1) {
            sum += atoi(buffer);
        } else {
            printf("Error reading line %d\n", i + 1);
            break;
        }
    }

    fclose(fp);
    printf("Sum is %d\n",sum);

    return 0;
}