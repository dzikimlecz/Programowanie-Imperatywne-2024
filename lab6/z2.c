#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 0    // 1 - dla testow,  0 - dla oceny automatycznej

void read_vec(double x[], size_t n) {
  for(size_t i=0; i<n; ++i)  scanf("%lf",x++);
}

void print_vec(const double x[], size_t n) {
  for(size_t i=0; i<n; ++i) printf("%.4f ",x[i]);
  printf("\n");
}

void read_mat(double A[][SIZE], size_t m, size_t n) {
  for(size_t i=0; i<m; ++i) {
    for(size_t j=0; j<n; ++j)  scanf("%lf",&A[i][j]);
  }
}

void print_mat(const double A[][SIZE], size_t m, size_t n) {
  for(size_t i=0; i<m; ++i) {
    for(size_t j=0; j<n; ++j)  printf("%.4f ",A[i][j]);
    printf("\n");
  }
}

void swp(int* p1, int* p2) {
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.
double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
    int detNot0 = 1;
    double sign = 1.0;
    int perm[SIZE] = {};
    double vals[SIZE] = {};
    for (int i = 0; i < n; ++i) {
        perm[i] = i;
        if (b) {
            vals[i] = b[i];
        }
    }
    for (int i = 0; detNot0 && i < n; ++i) {
        int maxi = i;
        for (int j = i + 1; j < n; ++j) {
            if (fabs(A[perm[j]][i]) > fabs(A[perm[maxi]][i])) {
                maxi = j;
            }
        }
        if (maxi != i) {
            swp(perm + i, perm + maxi);
            sign *= -1;
        }
        detNot0 = fabs(A[perm[i]][i]) >= eps;
        if (detNot0) {
            for (int j = i + 1; j < n; ++j) {
                double q = A[perm[j]][i] / A[perm[i]][i];
                if (q) {
                    for (int k = i; k < n; ++k) {
                        A[perm[j]][k] -= q * A[perm[i]][k];
                    }
                    vals[perm[j]] -= q * vals[perm[i]];
                }
            }
        }
    }

    double det = detNot0 ? sign : 0.0;
    for (int i = 0; det && i < n; ++i) {
        det *= A[perm[i]][i];
    }

    if (det && b && x) {
        for (int revi = 1; revi <= n; ++revi) {
            int i = n - revi;
            vals[perm[i]] /= A[perm[i]][i];
            for (int revj = revi + 1; revj <= n; ++revj) {
                int j = n - revj;
                double q = A[perm[j]][i] / A[perm[i]][i];
                vals[perm[j]] -= A[perm[j]][i] * vals[perm[i]];
                A[perm[j]][i] -= q * A[perm[i]][i];
            }
        }
        for (int i = 0; i < n; ++i) {
            x[i] = vals[perm[i]];
        }
    }

    return det;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
    double b[SIZE][SIZE];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            b[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    int detNot0 = 1;
    double sign = 1.0;
    int perm[SIZE] = {};
    for (int i = 0; i < n; ++i) {
        perm[i] = i;
    }

    for (int i = 0; detNot0 && i < n; ++i) {
        int maxi = i;
        for (int j = i + 1; j < n; ++j) {
            if (fabs(A[perm[j]][i]) > fabs(A[perm[maxi]][i])) {
                maxi = j;
            }
        }
        if (maxi != i) {
            swp(perm + i, perm + maxi);
            sign *= -1;
        }
        detNot0 = fabs(A[perm[i]][i]) >= eps;
        if (detNot0) {
            for (int j = i + 1; j < n; ++j) {
                double q = A[perm[j]][i] / A[perm[i]][i];
                if (q) {
                    for (int k = 0; k < n; ++k) {
                        A[perm[j]][k] -= q * A[perm[i]][k];
                        b[perm[j]][k] -= q * b[perm[i]][k];
                    }
                }
            }
        }
    }

    double det = detNot0 ? sign : 0.0;
    for (int i = 0; det && i < n; ++i) {
        det *= A[perm[i]][i];
    }

    if (det && B) {
        for (int revi = 1; revi <= n; ++revi) {
            int i = n - revi;
            for (int j = 0; j < n; j++) {
                b[perm[i]][j] /= A[perm[i]][i];
            }
            for (int revj = revi + 1; revj <= n; ++revj) {
                int j = n - revj;
                double q = A[perm[j]][i] / A[perm[i]][i];
                for (int k = 0; k < n; k++) {
                    b[perm[j]][k] -= A[perm[j]][i] * b[perm[i]][k];
                }
                A[perm[j]][i] -= q * A[perm[i]][i];
            }
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; j++) {
                B[i][j] = b[perm[i]][j];
            }
        }
    }

    return det;
}


int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;
    int to_do;
    size_t m, n, p, q;
    if (TEST) printf("Wpisz numer testu ");
    scanf("%d", &to_do);
    switch (to_do) {
        case 4:
            if (TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
            scanf("%d", &n);
            if (TEST) printf("Wpisz macierz A (wierszami): ");
            read_mat(A,n,n);
            if (TEST) printf("Wpisz wektor b: ");
            read_vec(b,n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n",det);
            if (det) print_vec(x,n);
            break;
        case 5:
            if (TEST) printf("Wpisz rozmiar macierzy n = ");
            scanf("%d",&n);
            if (TEST) printf("Wpisz elementy macierzy (wierszami): ");
            read_mat(A,n,n);
            det = matrix_inv(A,B,n,eps);
            printf("%.4f\n",det);
            if (det) print_mat(B,n,n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
