#include <stdio.h>
double pierw(double x, double precision);

int main() {
    printf("Podaj liczbę: ");
    double d = 0.0;
    scanf("%lf", &d);
    if (d < 0) {
        printf("Program oblicza wyłącznie pierwiastki liczb dodatnich.");
        return 1;
    }
    printf("Podaj żądaną precyzję ");
    double precision = 0.0;
    scanf("%lf", &precision);
    if (precision < 0) {
        printf("Precyzja musi być dodatnia");
        return 1;
    }
    printf("sqrt(%lf) = %lf\n", d, pierw(d, precision));
    return 0;
}

double pierw(double x, double precision) {
    double root = x;
    while (precision < (root * root - x)) {
        root += x/root;
        root /= 2.0;
    }
    return root;
}
