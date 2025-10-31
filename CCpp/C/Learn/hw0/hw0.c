#include <stdio.h>


char *quotes[]= {
    "noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n",
    "bortaS bIr jablu'DI' reH QaQqu' nay'.\n",
    "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n",
    "bISeH'eghlaH'be'chugh latlh Dara'laH'be'.\n",
    "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n",
    "Suvlu'taHvIS yapbe' HoS neH.\n",
    "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n",
    "Heghlu'meH QaQ jajvam.\n",
    "leghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n",
};

int main(){
    long a;
    printf("%s","ml' nob:\n");
        if (scanf(" %ld", &a) != 1 || getchar() != '\n'){ //skontroluje ci je tam number a potom getchar skontroluje dalsi znak
            printf("%s","Neh mi'\n");
            return 0;
        } else {
            switch (a) {
                    case 0:
                        printf("%s","Qapla'\n");
                        printf("%s", quotes[0]);
                        break;
                    case 1:
                        printf("%s","Qapla'\n");
                        printf("%s", quotes[1]);
                        break;
                    case 2:
                        printf("%s","Qapla'\n");
                        printf("%s", quotes[2]);
                        break;
                    case 3:
                        printf("%s","Qapla'\n");
                        printf("%s", quotes[3]);
                        break;
                    case 4:
                        printf("%s","Qapla'\n");
                        printf("%s", quotes[4]);
                        break;
                    case 5:
                        printf("%s","Qapla'\n");
                        printf("%s", quotes[5]);
                        break;
                    case 6:
                        printf("%s","Qapla'\n");
                        printf("%s", quotes[6]);
                        break;
                    case 7:
                        printf("%s","Qapla'\n");
                        printf("%s", quotes[7]);
                        break;
                    case 8:
                        printf("%s","Qapla'\n");
                        printf("%s", quotes[8]);
                        break;
                    default:
                        printf("Qih mi' %ld\n", a);
                        break;
                }
            return 0;
        }
 

    return 0;
}