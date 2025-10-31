// #include <stdio.h>
// #include <string.h>

// int main() {
//     const char *quotes[] = {
//         "Qapla'",
//         "bortaS bIr jablu'DI' reH QaQqu' nay'.",
//         "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.",
//         "bISeH'eghlaH'be'chugh latlh Dara'laH'be'.",
//         "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.",
//         "Suvlu'taHvIS yapbe' HoS neH.",
//         "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.",
//         "Heghlu'meH QaQ jajvam.",
//         "leghlaHchu'be'chugh mIn lo'laHbe' taj jej."
//     };

//     char input[100];
//     int a;
    
//     printf("%s", "ml' nob:\n");
//     scanf("%s", input);

//     if (strchr(input, '.') != NULL) {
//         printf("Qih mi' %s\n",input);
//         return 1;
//     }
    
//     if (sscanf(input, "%d", &a) != 1) {
//         printf("Neh mi'\n");
//         return 1;
//     }

//     switch (a) {
//         case 0: case 1: case 2: case 3: case 4:
//         case 5: case 6: case 7: case 8:
//             printf("%s\n", quotes[a]); 
//             break;
//         default:
//             printf("Qih mi' %d\n", a);
//             break;
//     }

//     return 0;
// }

#include <stdio.h>
#include <string.h>


int checkAscii(const char *array) {
    for (int i = 0; array[i] != '\0'; i++) {
        if ((unsigned char)array[i] > 127) {
            return 0;
        }
    }
    return 1; 
}

int main() {
    const char *quotes[] = {
        "Qapla'",
        "bortaS bIr jablu'DI' reH QaQqu' nay'.",
        "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.",
        "bISeH'eghlaH'be'chugh latlh Dara'laH'be'.",
        "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.",
        "Suvlu'taHvIS yapbe' HoS neH.",
        "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.",
        "Heghlu'meH QaQ jajvam.",
        "leghlaHchu'be'chugh mIn lo'laHbe' taj jej."
    };

    char input[100];
    int a;
    
    printf("%s", "ml' nob:\n");
    scanf("%s", input); 


    if (!checkAscii(input)) {
        printf("Neh mi' - Invalid characters detected\n");
        return 1;
    }

    if (strchr(input, '.') != NULL) {
        printf("Qih mi' %s\n", input);
        return 1;
    }

    if (sscanf(input, "%d", &a) != 1) {
        printf("Neh mi'\n");
        return 1;
    }


    switch (a) {
        case 0: case 1: case 2: case 3: case 4:
        case 5: case 6: case 7: case 8:
            printf("%s\n", quotes[a]); 
            break;
        default:
            printf("Qih mi' %d\n", a); 
            break;
    }

    return 0;
}