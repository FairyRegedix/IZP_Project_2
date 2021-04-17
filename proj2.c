#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define I_0 1.e-12 // saturacny prud
#define U_T 25.8563e-3 // tepelne napatie

/* IZP Projekt 2 - Iteracni vypocty
** autor: Patrik Demsky
** login: xdemsk00
** datum: 17.11.2019
*/

double load(char *arg);

double diode(double u0, double r, double eps);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fputs("wrong number of arguments\n", stderr);
        return 1;
    }

    // nacitat agrumenty
    double U_0 = load(argv[1]); // hodnota vstupneho napatia vo Voltoch
    double R = load(argv[2]); // odpor rezistoru v Ohmoch
    double EPS = load(argv[3]); // absolutna chyba/prestnost/odchylka

    // overit ci argumenty davaju zmysel
    if (U_0 < 0) {
        fputs("error: invalid arguments\n", stderr);
        return 1;
    }

    if (R <= 0) {
        fputs("error: invalid arguments\n", stderr);
        return 1;
    }

    // vypocitat hodnoty
    double U_P = diode(U_0, R, EPS);
    double I_P = I_0 * (exp(U_P / U_T) - 1);

    // vypis hodnot
    printf("Up=%g V\n", U_P);
    printf("Ip=%g A\n", I_P);
    return 0;
}

double diode(double u0, double r, double eps) {
    // deklaracia hodnot
    double a = 0; // zaciatok
    double b = u0; // koniec
    double c = b; // stred
    int i = 0; // pocet iteraci
    // cyklus polenia
    while ((b - a) >= eps) {
        // hladanie stredu
        c = (a + b) / 2;
        // spravnu krajnu hodnotu nahradza stredom
        if (I_0 * (exp(c / U_T) - 1) - ((u0 - c) / r) > 0) {
            b = c;
        } else {
            a = c;
        }
        i++;
        // zastane po vykonani 100 iteraci
        if (i > 100) {
            break;
        }
    }
    return c;
}

// funkcia overuje, ci maju argumenty zmysel
double load(char *arg) {
    double b = 0;
    char *endptr;
    b = strtod(arg, &endptr);
    if (*endptr != '\0') {
        fputs("error: invalid argument\n", stderr);
        exit(1);
    }
    return b;
}