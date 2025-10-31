#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/*
int main() {
    char password[50];  // Buffer for the password
    int index = 0;
    char c;

    printf("Enter your password: ");
    
    // Read characters one by one and mask them
    while ((c = getchar()) != '\n' && c != EOF && index < sizeof(password) - 1) {
        password[index++] = c;
        printf("*");  // Mask the input with an asterisk
    }

    password[index] = '\0';  // Null-terminate the string

    printf("\nYour password is securely stored.\n");

    return 0;
}

int main(){
    char floating[10];
    char exponent[10];
    float sol = 1;

    scanf("%9[^e]e%9s",floating,exponent);

    for(int i = 0; i< atoi(exponent); i++){
        sol *= atof(floating);
    }
    printf("%.2f\n",sol);
    return 0;
}

int main(){
    char value[10];
    char key[10];

    scanf("%9[^=]=%9s",key,value);
    printf("K: %s, V: %s\n",key,value);
    return 0;
}

int main(){
    int matrix[3][3];
    for(int i = 0; i < 3;i++){
        for(int j=0; j<3;j++){
            scanf(" %d",&matrix[i][j]);
        }
    }

    for(int i = 0; i < 3;i++){
        for(int j=0; j<3;j++){
            printf(" %d",matrix[j][i]);
        }
        printf("\n");
    }

    return 0;
}

int main(){
    int age;
    float score;
    char name[100];

    scanf("%99s%d%f",name,&age,&score);
    printf("Name: %s, Age: %d, Score: %.2f",name,age,score);

    return 0;
}

int main(){
    int list[3];

    scanf("%d,%d,%d",&list[0],&list[1],&list[2]);
    printf("%d\n",list[0]+list[1]+list[2]);
    return 0;
}

int main(){
    char veta[100];

    scanf("%[^\n]s",veta);
    printf("%s",veta);
    return 0;
}


int main(){
    int a,b;

    scanf("%d %*d %d",&a,&b);
    printf("A:%d B:%d\n",a,b);
    return 0;
}

int main(){
    int a =__INT_MAX__;

    while (1){
        if(scanf("%d",&a)==1){
            break;
        } else {
            printf("Invalid input. Please enter an integer: ");
            while (getchar() != '\n');
        }
    }
    printf("You have entered: %d\n",a);
    return 0;
}

int main(){
    int in;
    float fl;
    char ch[100];
    int nulls = 0;

    scanf("%d%f%99s",&in,&fl,ch);

    printf("Intiger: %d\n",in);
    printf("Float: %.2f\n",fl);
    printf("String: %s\n",ch);
    return 0;
}


#define maxSize 10

int main(){
    char string[maxSize][maxSize];

    int size=0;
    
    while(1){
        scanf("%s",string[size]);

        if(strcmp(string[size],"stop")==0){
            break;
        }
        size++;

        if(size == maxSize){
            printf("Maximum size reached\n");
            break;
        }
    }

    for(int i = 0; i < size; i++){
        printf("%s\n",string[i]);
    }

    return 0;
}
int main(){
    int hex,oct,dec;

    scanf("%x %o %d", &hex, &oct, &dec);

    printf("Hex: %d, Oct: %d, Dec: %d\n",hex,oct,dec);

    return 0;
}


int main(){
    int a,b = 0;
    char input[100];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Invalid input.\n");
        return 1;
    }

    int count = sscanf(input,"%d %d",&a,&b);

    if(count ==1){
        printf("one entered\n");
    } else if (count ==2){
        printf("two entered\n");
    }
    
    printf("a:%d b:%d\n",a,b);
    return 0;
}


int main(){
    char *string=NULL;
    int cap = 10;
    string = (char *)malloc(cap*sizeof(char));
    if(string == NULL){
        printf("No string\n");
        return 0;
    }
    int size = 0;
    char a;


    while (1){
        scanf("%c",&a);
        if(a == '#'){
            printf("Ending program\n");
            break;
        }

        if(size == cap){
            cap*=2;
            char *stringisko = (char *)realloc(string,cap*sizeof(char));
            if(stringisko == NULL){
                printf("No stringisko\n");
                free(string);
                return 0;
            }
            string = stringisko;
        }
        string[size]=a;
        size++;
    }

    for(int i = 0; i< strlen(string);i++){
        printf("%c",string[i]);
        if(i==strlen(string)){
            printf("\n");
        }
    }
    free(string);
    return 0;
}


int main(){
    char pismena[5];
    scanf("%5c",pismena);

    printf("%s\n",pismena);
    return 0;
}


int main(){
    char slovo1[100],slovo2[100],slovo3[100];
    if(scanf("%99s %99s %99s",slovo1,slovo2,slovo3)!=3){
        printf("Zly format\n");
        return 0;
    }
    
    printf("%s %s %s\n",slovo1,slovo2,slovo3);

    return 0;
}


int main(){
    int day,month,year;
    if(scanf("%4d-%2d-%2d",&year,&month,&day)!=3 || year < 1000 || month > 12 || day > 31 || month <0|| year < 0 || day<0){
        printf("Zly format alebo udaje\nSpravne: YYY-MM-DD\n");
        return 0;
    }
    printf("Day: %d, Month: %d, Year: %d\n",day,month,year);
    return 0;
}


int main(){
    int a;
    char *string=NULL;
    int cap = 10;

    string = (char *)malloc(cap*sizeof(char));
    scanf("%d%s",&a,string);

    printf("Intiger: %d, String: %s\n",a,string);

    return 0;
}


int main(){
    const char vowels[]={'a','e','i','o','u'};
    int vowel = 0;

    char a[];
    scanf("%c",&a);

    if(memchr(vowels,a,strlen(vowels))){
        printf("vowel\n");
        vowel = 1;
    }

    if(vowel == 0){
        printf("constant\n");
    }

    return 0;
}


int main(){
    char *veta=NULL;
    int cap = 10;

    veta = (char *)malloc(cap*sizeof(char));
    if(veta==NULL){
        printf("Failed to allocate memory\n");
        return 0;
    }
    scanf("%[^\n]%*c",veta);

    for(int i = strlen(veta)-1; i>=0;i--){
        printf("%c",veta[i]);
    }
    printf("\n");
    return 0;
} */