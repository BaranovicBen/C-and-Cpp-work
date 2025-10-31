#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_GRID 100000
#define MAX_WORD 10000

/* V poli direction su ulozene vsetky mozne smery, v ktorych mozeme prehladavat
   osemsmerovku. Kazdy riadok obsahuje posun v riadkoch a stlpcoch. */
int direction[4][2] = {
    {0, 1},
    {1, 0},
    {1, 1},
    {1, -1}
};

/* Struktura crossword (krizovka) uchovava rozmery hadanky (row, col), 2D mriežku znakov (grid)
   a 2D pole celých čísel (crossed), do ktoreho sa zaznamenávajú označené (vyškrtané) písmená. */
typedef struct {
    int row;
    int col;
    char **grid;
    int **crossed;
} crossword;

void freeAll(crossword *game);
int readMatrix(crossword *game);
int searchSol(crossword *game, const char *word, int mark);
void printRem(crossword *game);
int isLowerCheck(const char *word);
int separator(char c);

int separator(char c) {
    if (c == '\0' || isspace((unsigned char)c) || ispunct((unsigned char)c)) {
        return 1;
    }
    return 0;
}

/* Funkcia freeAll uvoľní všetku dynamicky alokovanú pamäť. Najprv uvoľní riadky gridu,
   potom samotný grid a nakoniec uvoľní aj pole crossed aj s jeho riadkami. */
void freeAll(crossword *game) {
    int i;
    if (game->grid != NULL) {
        for (i = 0; i < game->row; i++) {
            if (game->grid[i] != NULL) {
                free(game->grid[i]);
            }
        }
        free(game->grid);
    }
    if (game->crossed != NULL) {
        for (i = 0; i < game->row; i++) {
            if (game->crossed[i] != NULL) {
                free(game->crossed[i]);
            }
        }
        free(game->crossed);
    }
    game->grid = NULL;
    game->crossed = NULL;
    game->row = 0;
    game->col = 0;
}

/* Funkcia readMatrix načíta a spracuje vstup až po prázdny riadok.
   Skontroluje, či má každý riadok rovnakú dĺžku a obsahuje len malé písmená alebo bodky.
   Ak je niečo nesprávne (napr. veľké písmeno alebo rôzne dĺžky riadkov),
   funkcia uvoľní alokovanú pamäť a vráti 0.
   Ak je všetko v poriadku, alokuje a inicializuje crossed a vráti 1. */
int readMatrix(crossword *game) {
    char buffer[MAX_GRID];
    game->row = 0;
    game->grid = NULL;
    game->crossed = NULL;

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        if (buffer[0] == '\n') {
            break;
        }

        int len = (int)strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        if (game->row == 0) {
            game->col = len;
        } else {
            if (len != game->col) {
                freeAll(game);
                return 0;
            }
        }

        {
            int i;
            for (i = 0; i < len; i++) {
                if (!islower((unsigned char)buffer[i]) && buffer[i] != '.') {
                    freeAll(game);
                    return 0;
                }
            }
        }

        {
            char **tmp_grid = (char **)realloc(game->grid, sizeof(char *) * (game->row + 1));
            if (tmp_grid == NULL) {
                freeAll(game);
                return 0;
            }
            game->grid = tmp_grid;
        }

        {
            char *rowStr = (char *)malloc(len + 1);
            if (rowStr == NULL) {
                freeAll(game);
                return 0;
            }
            strcpy(rowStr, buffer);
            game->grid[game->row] = rowStr;
        }

        game->row++;
    }

    if (game->row == 0) {
        freeAll(game);
        return 0;
    }

    game->crossed = (int **)malloc(game->row * sizeof(int *));
    if (game->crossed == NULL) {
        freeAll(game);
        return 0;
    }

    {
        int i;
        for (i = 0; i < game->row; i++) {
            game->crossed[i] = NULL;
        }
        for (i = 0; i < game->row; i++) {
            game->crossed[i] = (int *)calloc((size_t)game->col, sizeof(int));
            if (game->crossed[i] == NULL) {
                freeAll(game);
                return 0;
            }
        }
    }

    return 1;
}

/* Funkcia isLowerCheck overí, či je dané slovo písané iba malými písmenami
   a má aspoň dĺžku 2 znaky. Ak áno, vráti 1, inak 0. */
int isLowerCheck(const char *word) {
    size_t length = strlen(word);
    if (length < 2) {
        return 0;
    }

    const char *ch = word;
    while (*ch != '\0') {
        if (!islower((unsigned char)*ch)) {
            return 0;
        }
        ch++;
    }

    return 1;
}

/* Funkcia searchSol prehľadá celú mriežku a hľadá dané slovo vo všetkých smeroch
   definovaných v direction, a to v normálnom aj obrátenom poradí.
   Ak parametr 'mark' je 1, nájdené slová označí v 'crossed', inak ich len spočíta.
   Vráti počet nájdených výskytov slova. */
int searchSol(crossword *game, const char *word, int mark) {
    int found = 0;
    int wordLen = (int)strlen(word);

    int r;
    for (r = 0; r < game->row; r++) {
        int c;
        for (c = 0; c < game->col; c++) {
            char firstChar = word[0];
            char lastChar = word[wordLen - 1];
            char currentGridChar = game->grid[r][c];

            if (currentGridChar != firstChar && currentGridChar != lastChar) {
                continue;
            }

            {
                int d;
                for (d = 0; d < 4; d++) {
                    int reverse;
                    for (reverse = 0; reverse < 2; reverse++) {
                        int row = r;
                        int col = c;
                        int matched = 1;
                        int i;

                        for (i = 0; i < wordLen; i++) {
                            char expected;
                            if (reverse == 1) {
                                expected = word[wordLen - 1 - i];
                            } else {
                                expected = word[i];
                            }

                            if (row < 0 || row >= game->row || col < 0 || col >= game->col) {
                                matched = 0;
                                break;
                            }

                            if (game->grid[row][col] != expected) {
                                matched = 0;
                                break;
                            }

                            row += direction[d][0];
                            col += direction[d][1];
                        }

                        if (matched == 1) {
                            found++;
                            if (mark == 1) {
                                int rr = r;
                                int cc = c;
                                int j;
                                for (j = 0; j < wordLen; j++) {
                                    game->crossed[rr][cc] = 1;
                                    rr += direction[d][0];
                                    cc += direction[d][1];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return found;
}


/* Funkcia printRem (print Remaining) vypíše všetky nevyškrtané písmená (okrem bodiek) po 60 znakov na riadok.
   Tým umožní zobraziť tzv. tajenku, ktorá zostane po odstránení nájdených slov. */
void printRem(crossword *game) {
    int lineLength = 0;
    int r;
    for (r = 0; r < game->row; r++) {
        int c;
        for (c = 0; c < game->col; c++) {
            if (game->crossed[r][c] == 1 || game->grid[r][c] == '.') {
                continue;
            }

            putchar(game->grid[r][c]);
            lineLength++;
            if (lineLength == 60) {
                putchar('\n');
                lineLength = 0;
            }
        }
    }
    if (lineLength > 0) {
        putchar('\n');
    }
}

/* Vo funkcii main sa najprv inicializuje hra (mriežka) pomocou readMatrix.
   Potom sa čítajú príkazy:
    -  [slovo]: nasleduje slovo, ktoré sa hľadá a zároveň označí v mriežke. 
      Program vypíše, koľkokrát bolo slovo nájdené.
    # [slovo]: nasleduje slovo sa len hľadá (neoznačí), a vypíše sa počet nájdení.
    ?: vypíše sa tzv. tajenka z neoznačených písmen.
   Pri akýchkoľvek nesprávnych vstupoch sa program skončí s chybovou hláškou.

   Nakoniec sa uvoľní pamäť a program skončí.
*/

int main() {
    crossword game;
    game.row = 0;
    game.col = 0;
    game.grid = NULL;
    game.crossed = NULL;

    printf("Osmismerka:\n");
    if (readMatrix(&game) == 0) {
        printf("Nespravny vstup.\n");
        freeAll(&game);
        return 1;
    }

    char command[MAX_WORD];
    while (1) {
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        if (command[0] == '-') {
            char word[MAX_WORD];
            char *ptr = command + 1;
            while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
                ptr++;
            }

            int num_chars_read = 0;
            int scanned = sscanf(ptr, "%s%n", word, &num_chars_read);
            if (scanned != 1 || isLowerCheck(word) == 0) {
                printf("Nespravny vstup.\n");
                freeAll(&game);
                return 1;
            }

            ptr += num_chars_read;
            while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
                ptr++;
            }

            if (*ptr != '\0') {
                printf("Nespravny vstup.\n");
                freeAll(&game);
                return 1;
            }

            int found = searchSol(&game, word, 1);
            printf("%s: %dx\n", word, found);
        } else if (command[0] == '#') {
            char word[MAX_WORD];
            char *ptr = command + 1;
            while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
                ptr++;
            }

            int num_chars_read = 0;
            int scanned = sscanf(ptr, "%s%n", word, &num_chars_read);
            if (scanned != 1 || isLowerCheck(word) == 0) {
                printf("Nespravny vstup.\n");
                freeAll(&game);
                return 1;
            }

            ptr += num_chars_read;
            while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
                ptr++;
            }

            if (*ptr != '\0') {
                printf("Nespravny vstup.\n");
                freeAll(&game);
                return 1;
            }

            int found = searchSol(&game, word, 0);
            printf("%s: %dx\n", word, found);
        } else if (command[0] == '?') {
            char *ptr = command + 1;
            while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
                ptr++;
            }

            if (*ptr != '\0') {
                printf("Nespravny vstup.\n");
                freeAll(&game);
                return 1;
            }

            printf("Tajenka:\n");
            printRem(&game);
        } else {
            printf("Nespravny vstup.\n");
            freeAll(&game);
            return 1;
        }
    }

    freeAll(&game);
    return 0;
}