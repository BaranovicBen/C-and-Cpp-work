#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <float.h>


void skip_whitespace() 
{
    char ch;
    while (isspace(ch = getchar()));
    ungetc(ch, stdin); 
}

int read_point(long double *x, long double *y) 
{
    char ch;
    skip_whitespace();
    if ((ch = getchar()) != '[') {
        printf("Nespravny vstup.\n");
        return 0;
    }
    skip_whitespace();
    if (scanf("%Lf", x) != 1) {
        printf("Nespravny vstup.\n");
        return 0;
    }
    skip_whitespace();
    if ((ch = getchar()) != ',') {
        printf("Nespravny vstup.\n");
        return 0;
    }
    skip_whitespace();
    if (scanf("%Lf", y) != 1) {
        printf("Nespravny vstup.\n");
        return 0;
    }
    skip_whitespace();
    if ((ch = getchar()) != ']') {
        printf("Nespravny vstup.\n");
        return 0;
    }

    return 1;
}

long double distance(long double x1, long double y1, long double x2, long double y2) {
    return sqrtl((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

long double angle_between_vectors(long double v1_x, long double v1_y, long double v2_x, long double v2_y) {
    long double dot_product = v1_x * v2_x + v1_y * v2_y;
    long double magnitude_v1 = sqrtl(v1_x * v1_x + v1_y * v1_y);
    long double magnitude_v2 = sqrtl(v2_x * v2_x + v2_y * v2_y);
    return acos(dot_product / (magnitude_v1 * magnitude_v2)) * (180.0 / M_PI);
}

void print_number(long double num) {
    char buffer[100];
    
    if (fabsl(num) >= 1e6 || fabsl(num) < 1e-3) {
        sprintf(buffer, "%.5Le", num);
    } else {
        sprintf(buffer, "%.10Lf", num);
    
        char *p = buffer + strlen(buffer) - 1;
        while (*p == '0') {
            *p-- = '\0';
        }
        if (*p == '.') {
            *p = '\0';
        }
    }
        printf("%s", buffer);
}



int main() {
    long double Ax, Ay, Bx, By, Cx, Cy;

    printf("Bod A:\n");
    if (!read_point(&Ax, &Ay)) {
        return 1;
    }

    printf("Bod B:\n");
    if (!read_point(&Bx, &By)) {
        return 1;
    }

    printf("Bod C:\n");
    if (!read_point(&Cx, &Cy)) {
        return 1;
    }

    long double max_coord = fmaxl(fmaxl(fabsl(Ax), fabsl(Ay)), fmaxl(fabsl(Bx), fmaxl(fabsl(By), fmaxl(fabsl(Cx), fabsl(Cy)))));

    long double Ax_norm = Ax / max_coord;
    long double Ay_norm = Ay / max_coord;
    long double Bx_norm = Bx / max_coord;
    long double By_norm = By / max_coord;
    long double Cx_norm = Cx / max_coord;
    long double Cy_norm = Cy / max_coord;

    long double determinant = (Bx_norm - Ax_norm) * (Cy_norm - Ay_norm) - (By_norm - Ay_norm) * (Cx_norm - Ax_norm);

    if (fabsl(determinant) <= DBL_EPSILON * 1000) {
        printf("Rovnobezniky nelze sestrojit.\n");
        return 0;
    }


    long double Dx = Bx + (Cx - Ax), Dy = By + (Cy - Ay), Ex = Cx + (Ax - Bx), Ey = Cy + (Ay - By), Fx = Ax + (Bx - Cx), Fy = Ay + (By - Cy); 

    long double a = distance(Ax, Ay, Bx, By);
    long double b = distance(Ax, Ay, Cx, Cy);

    long double vector_AB_x = Bx - Ax, vector_AB_y = By - Ay;
    long double vector_AC_x = Cx - Ax, vector_AC_y = Cy - Ay;

    long double alfa = angle_between_vectors(vector_AB_x, vector_AB_y, vector_AC_x, vector_AC_y);

    printf("A': [");
    print_number(Dx);
    printf(",");
    print_number(Dy);
    if (fabsl(alfa - 90) <= alfa * 10000 * DBL_EPSILON) {
        if (fabsl(a - b) <= (a+b) * 10000 * DBL_EPSILON) {
            printf("], ctverec\n");
        } else {
            printf("], obdelnik\n");
        }
    } else if (fabsl(a - b) <= (a+b) * 10000 * DBL_EPSILON) {
        printf("], kosoctverec\n");
    } else {
        printf("], rovnobeznik\n");
    }

    long double a1 = distance(Ax, Ay, Bx, By);
    long double b1 = distance(Ax, Ay, Ex, Ey);

    long double vector_AE_x = Ex - Ax, vector_AE_y = Ey - Ay;

    long double beta = angle_between_vectors(vector_AB_x, vector_AB_y, vector_AE_x, vector_AE_y);


    printf("B': [");
    print_number(Ex);
    printf(",");
    print_number(Ey);
    if (fabsl(beta - 90) <= beta * 10000 * DBL_EPSILON) {
        if (fabsl(a1 - b1) <= (a1+b1) * 10000 * DBL_EPSILON) {
            printf("], ctverec\n");
        } else {
            printf("], obdelnik\n");
        }
    } else if (fabsl(a1 - b1) <= (a1+b1) * 10000 * DBL_EPSILON) {
        printf("], kosoctverec\n");
    } else {
        printf("], rovnobeznik\n");
    }

    long double a2 = distance(Ax, Ay, Fx, Fy);
    long double b2 = distance(Bx, By, Fx, Fy);

    long double vector_AF_x = Fx - Ax, vector_AF_y = Fy - Ay;
    long double vector_BF_x = Fx - Bx, vector_BF_y = Fy - By;
    long double gama = angle_between_vectors(vector_AF_x, vector_AF_y, vector_BF_x, vector_BF_y);

    printf("C': [");
    print_number(Fx);
    printf(",");
    print_number(Fy);
    if (fabsl(gama - 90) <= gama * 10000 * DBL_EPSILON) {
        if (fabsl(a2 - b2) <= (a2+b2) * 10000 * DBL_EPSILON) {
            printf("], ctverec\n");
        } else {
            printf("], obdelnik\n");
        }
    } else if (fabsl(a2 - b2) <= (a2+b2) * 10000 * DBL_EPSILON) {
        printf("], kosoctverec\n");
    } else {
        printf("], rovnobeznik\n");
    }

    return 0;
}
