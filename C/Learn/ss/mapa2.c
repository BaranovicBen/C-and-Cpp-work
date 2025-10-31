#include <stdio.h>

struct pozemok{
    int cena;
    int chX;
    int chY;
};

int main(){
    int a,b;

    scanf("%d %d",&a,&b);
    if(a <= 0 || b <= 0){
        printf("Nespravny vstup.\n");
        return 0;
    }

    int array[a][b];

    for (int i = 0; i < b; i++){
        for(int j = 0; j< a; j++){
            scanf("%d", &array[i][j]);
        }
    }
    int x,y,width,height;
    if(scanf("%d %d %d %d",&x,&y,&width,&height)!=4){
        printf("Nespravny vstup.\n");
        return 0;
    }

    if(width >a || height >b || x > a || y > b ){
        printf("Neexistuje.\n");
        return 0;
    } else if (width <= 0 || height <= 0 || x < 0 || y < 0 ){
        printf("Neexistuje.\n");
        return 0;
    }
    
    int cheapest = __INT_MAX__;
    struct pozemok Pozemky[100];
    int counter = 0;
    
    for(int startY=y-height+1;startY <=y && startY >=0 && startY+height <= a; startY++){
        for (int startX = x - width + 1; startX <= x && startX >= 0 && startX + width <= b; startX++) {
            int sumPozemok = 0;

            for(int i = 0; i< height;i++){
                for(int j = 0; j < width; j++){
                    sumPozemok += array[startY+i][startX+j];
                }
            }
            if (sumPozemok <= cheapest) {
                cheapest = sumPozemok;
                Pozemky[counter].cena = sumPozemok;
                Pozemky[counter].chX = startX;
                Pozemky[counter].chY = startY;
                counter += 1;
            }
        }
    }

    if (cheapest == __INT_MAX__){
        printf("Neexistuje.\n");
    } else {
        printf("Cena: %d\n", cheapest-array[y][x]);
        for(int i = 0; i < counter; i++){
            printf("* (%d,%d)\n",Pozemky[i].chX,Pozemky[i].chY);
        }
    }
        

    return 0;
}