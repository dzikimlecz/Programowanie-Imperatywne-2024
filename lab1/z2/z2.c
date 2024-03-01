#include <stdio.h>
#include <stdlib.h>

int nwd(int a, int b) {
    while (b) {
        int c = a % b;
        a = b;
        b = c;
    }
    return a;
}

int main() {
    int a, b;
    a = b = 0;
    printf("Podaj liczby: ");
    scanf("%d %d", &a, &b);
    if (a < 0 || b < 0) {
        printf("Liczby muszą być nieujemne.");
        exit(0);
    }
    printf("NWD = %d\n", nwd(a, b));
    return 0;
}
