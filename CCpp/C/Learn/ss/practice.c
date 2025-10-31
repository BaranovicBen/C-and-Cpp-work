#include <stdio.h>
#include <stdlib.h>

int main(){

    int rows = 0;
    int columns =0;

    if(scanf("%d %d",&rows,&columns)!=2){
        printf("Nespravny vstup.\n");
        return 0;
    }

    int **mapa = (int **)malloc(rows * sizeof(int  *));
    for(int i = 0; i < rows;i++){
        mapa[i] = (int *)malloc(columns * sizeof(int));
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j<columns; j++){
            scanf("%d", &mapa[i][j]);
        }
    }


    

    for (int i = 0; i < rows; i++) {
        free(mapa[i]);
    }
    free(mapa);
    return 0;
}


/*
int main(){

 
    //double 8 bytes
    //calculate how many elements are in array: sizeof(array)/sizeof(array[0])
    //int numbers[riadky][stlpce]
    // int numbers[2][3]={
    //                     {1,2,3},
    //                     {5,6,7}
    //                     };


    int a,b;
    scanf("%d %d",&a,&b);
    int numbers[a][b];

    numbers[0][0]= 1;
    numbers[0][1]= 2;
    numbers[0][2]= 3;
    numbers[1][0]= 4;
    numbers[1][1]= 5;
    numbers[1][2]= 6;

    int rows = sizeof(numbers)/sizeof(numbers[0]);
    int collums = sizeof(numbers[0])/sizeof(numbers[0][0]);

    printf("Row %d, Columns %d\n",rows,collums);

    for(int i = 0; i< rows;i++){
        for(int j = 0; j <collums; j++){
            printf("%d ", numbers[i][j]);
        }
        printf("\n");
    }

    return 0;
}



int main(){
    int a,b,c,d;

    if(scanf("%d %d %d",&a,&b,&c)|| scanf("%d%d%d%d",&a,&b,&c,&d)){
        printf("%d %d %d %d",a,b,c,d);
    }

    return 0;

}

int primes(int *a, int *b){
    printf("Primes: ");
    for(int i = *a; i<= *b; i++){
        int prime = 1;
        if(i < 2) continue;

        for(int j = 2; j <= sqrt(i);j++){
            if(i%j ==0){
                prime = 0;
            }
        }

        if(prime){
            printf("%d ",i);
        }
    }
    printf("\n");
    return 0;
}

int main(){
    int start,end;
    scanf("%d %d",&start,&end);

    primes(&start,&end);

    return 0;
}

int pallindromer(char *string){
    int lenght = strlen(string);
    for(int i = 0; i < lenght/2;i++){
        if(string[i] != string[lenght-1-i]){
            printf("Not a palindrome\n");
            return 0;
        } 
    }
    printf("It is a palindrome\n");
    return 1;
}

int main(){
    char string[100];
    scanf("%s",string);
    pallindromer(string);
    return 0;
}

int main(){
    int n;
    scanf("%d",&n);

    for(int i = 1; i <= 9;i++){
        printf("%d *  %d | %d\n",n,i,i*n);
    }
    printf("%d * %d | %d\n",n,10,10*n);
    return 0;
}

int main(){
    char vowels[5]={'a','e','i','o','u'};
    char string[100];
    scanf("%[^\n]s",string);

    int count;

    for (int i = 0; i < strlen(string); i++) {
        for (int j = 0; j < 5; j++) {
            if (string[i] == vowels[j]) {
                count++; 
                break;
            }
        }
    }
    printf("Vowels in string: %d\n",count);
    return 0;
}

int main(){
    int array[9]={1,2,333,4,5,6,7,8,111};
    int largest = INT_MIN;
    int size = sizeof(array)/sizeof(array[0]);

    for(int i = 0; i<size;i++){
        if(array[i]>largest){
            largest = array[i];
        }
    }

    printf("Largest num is %d\n",largest);
    return 0;
}

int main(){
    char number[100];
    scanf("%s",number);
    
    for(int i = strlen(number); i >= 0; i--){
        printf("%c",number[i]);
    }
    printf("\n");
    return 0;
}

int main(){
    int n = 0;
    int sum=0;
    scanf("%d",&n);
    for(int i = 1; i <=n; i++){
        sum += i;
    }
    printf("%d\n",sum);
    return 0;
}

int main(){
    int n;
    scanf("%d",&n);
    for(int i = 1; i<= n;i++){
        printf("%d ",i);
    }
    printf("\n");
    return 0;
} */