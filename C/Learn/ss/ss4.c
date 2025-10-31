#include <stdio.h>

int gcd(int bigger, int whole){
        int tmp;
        
        if(bigger % whole == 0){
        printf("%d is GCD\n",whole);
            return whole; 
        }

        while(bigger > 0) {
            if (bigger - whole > 0){
                bigger -= whole;
            }
            if(bigger < whole) {
                tmp = bigger;
                bigger = whole;
                whole = tmp;
            }
            if(bigger - whole == 0){
                break;
            }
        }
        
        printf("%d is GCD\n",whole);
            return whole;
};

int main(){
    int x,y;
    printf("Give numbers for gcd\n");
    if(scanf(" %d %d",&x,&y)!=2){
        printf("Nespravny vstup.\n");
        return 0;
    }
    if(x<0 || y < 0){
        printf("Nespravny vstup.\n");
        return 0;   
    }

    if(x > y){
        gcd(x,y);
    } else if (y > x){
        gcd(y,x);
        return 0;
    } else if(x==y) {
        printf("%d is GCD\n",x);
        return 0;
    } else {
        printf("Nespravny vstup.\n");
        return 0;
    }

    return 0;
}

            // counter = bigger / whole;
            // printf("%d counter\n",counter);

            // remeinder = whole;
            // bigger =- counter*whole;
            // printf("%d bigger\n",bigger);

            // whole = bigger % whole;
            // printf("%d whole\n",whole);