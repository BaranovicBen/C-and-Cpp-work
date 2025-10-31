#include <stdio.h>

int main()
{
    int a;
    const char *quotes[] = 
    {
        "noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.",
        "bortaS bIr jablu'DI' reH QaQqu' nay'.",
        "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.",
        "bISeH'eghlaH'be'chugh latlh Dara'laH'be'.",
        "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.",
        "Suvlu'taHvIS yapbe' HoS neH.",
        "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.",
        "Heghlu'meH QaQ jajvam.",
        "leghlaHchu'be'chugh mIn lo'laHbe' taj jej."
    };

    printf("%s", "ml' nob:\n");
    
    if (scanf("%d", &a) != 1) 
    {
        printf("Neh mi'\n");
        return 1;
    }

    switch (a) 
    {
        case 0: case 1: case 2: case 3: case 4:
        case 5: case 6: case 7: case 8:
            printf("Qapla'\n%s\n", quotes[a]);
            break;
        default:
            printf("Qih mi' %d\n", a);
            break;
    }
    
    return 0;
}