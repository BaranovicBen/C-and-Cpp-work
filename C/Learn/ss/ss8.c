#include <stdio.h>
#include <string.h>


struct student {
    char name[10];
    char surname[10];
    int id;
    char grade;
} students;

int graider(char grade){
    switch(grade){
        case 'A':
            return 1;
        case 'B':
            return 2;
        case 'C':
            return 3;
        case 'D':
            return 4;
        case 'F':
            return 5;
        default:
            return 6;
    }
};

int main(){
    struct student students[5]= {
        {"Dano", "Drevo", 1, 'C'},
        {"Kim", "Petrakova", 10, 'A'},
        {"Peder", "Rysavec", 11, 'D'},
        {"Gabor", "Balaton", 100, 'A'},
        {"Sandor", "Fekete", 101, 'F'}
    };

    int bestGrade = 5;
    char bestStudent[10];

    for(int i = 1; i<=5; i++){
        if (graider(students[i].grade) < bestGrade){
            bestGrade = graider(students[i].grade);
            strcpy(bestStudent,students[i].name);
        }
    }

    printf("Best student is %s, with grade %d.\n",bestStudent,bestGrade);

    return 0;
}