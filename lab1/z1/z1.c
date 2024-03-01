#include <stdio.h>

void printFib(unsigned int n) {
    long long a0 = 0;
    long long a1 = 1;
    for (int i = 0; i < n; i++) {
        printf("%d ", a0);
        long long next = a1 + a0;
        a0 = a1;
        a1 = next;
    }
    printf("\n");
}

int main() {
    int n = 0;
    printf("How many elements are to be printed? ");
    scanf("%d", &n);
    if (n < 0) {
        printf("Cannot print negative ammount of elements.\n");
        return 0;
    }
    printFib(n);
}
