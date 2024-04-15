#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 0     // 1 - dla testow,  0 - dla oceny automatycznej

// 5.1.1

#define STRLEN_MAX  SIZE
#define KW_NO   32    // liczba zapisanych lancuchow (slow kluczowych)

#define str char*

// n lancuchów wskazywanych w tablicy wskaznikow ptab kopiuje do tablicy tablic t2D
void n_str_copy(char t2D[][STRLEN_MAX], char *ptab[], size_t n) {
    for (int i = 0; i < n; i++) {
        str s = ptab[i];
        int j;
        for (j = 0; *(s + j); ++j) {
            t2D[i][j] = *(s + j);
        }
        t2D[i][j] = 0;
    }
}


int compar(const void *p1, const void *p2) {
    const str* str1 = (const str*) p1;
    const str* str2 = (const str*) p2;
    return strcmp(*str1, *str2);
}


// sortuje alfabetycznie n lancuchow wskazywanych w tablicy wskaznikow t
void ptab_sort(char *ptab[], size_t n) {
    qsort(ptab, n, sizeof(char*), compar);
}

// Porzadek odwrotny do alfabetycznego lancuchow zapisanych w tablicy t2D zapisuje w tablicy indices
void t2D_sort(const char t2D[][STRLEN_MAX], size_t indices[], size_t n) {
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }
    int changed;
    do {
        changed = 0;
        for (int i = 1; i < n; i++) {
            if (strcmp(t2D[indices[i - 1]], t2D[indices[i]]) < 0) {
                int temp = indices[i];
                indices[i] = indices[i - 1];
                indices[i - 1] = temp;
                changed = 1;
            }
        }
    } while (changed);
}

// W wierszach tablicy t2D sa zapisane lancuchy znakowe w dowolnej kolejnosci.
// Tablica indeksow wierszy indices wyznacza porzadek tych lancuchow.
// Funkcja wypisuje w osobnych liniach łańcuchy wskazane przez n poczatkowych elementów tablicy indices.
void print_t2D_ind(const char (*ptr)[STRLEN_MAX], const size_t *pindices, size_t n) {
    for (int i = 0; i < n; i++) {
        int ix = pindices[i];
        const char* s = ptr[ix];
        for (int j = 0; s[j]; j++) {
            printf("%c", s[j]);
        }
        printf("\n");
    }
}

// Funkcja wypisuje w osobnych liniach n łańcuchów wskazywanych przez elementy tablicy ptab.
void print_ptab(char *ptab[], size_t n) {
    for (int i = 0; i < n; i++) {
        printf("%s\n", ptab[i]);
    }
}

// 5.1.2
// A mxp, B pxn

void mat_product(const double A[][SIZE], const double B[][SIZE], double AB[][SIZE], size_t m, size_t p, size_t n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            AB[i][j] = 0;
            for (int k = 0; k < p; k++) {
                AB[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


// 5.1.3 Triangulatyzacja macierzy i obliczanie wyznacznika - wersja uproszczona
// (bez zamiany wierszy).
// Jezeli element glowny a[i][i] = 0, to triangularyzacja nie jest dokonczona,
// a wyznacznik = NAN
// Zalozenie: funkcja gauss moze zmienic wartosci elementow tablicy A

double gauss_simplified(double A[][SIZE], size_t n) {
    int i;
    for (i = 0; A[i][i] && i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (A[j][i]) {
                double q = A[j][i] / A[i][i];
                for (int k = i; k < n; ++k) {
                    A[j][k] -= q * A[i][k];
                }
            }
        }
    }
    if (i < n) {
        return NAN;
    }
    double det = 1.0;
    for (int j = 0; j < n; j++) {
        det *= A[j][j];
    }
    return det;
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

int main(void) {
    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE];

	int to_do;
	size_t m,n,p,q;

	char *keywords_ptab[] = {"do", "struct", "typedef", "for", "union",
    "int", "void", "long", "register", "auto", "return",
    "double", "else", "sizeof", "enum", "const", "continue",
    "default", "short", "extern", "static", "float",
    "goto", "switch", "if", "unsigned", "volatile",
    "while", "signed", "break", "char", "case"};
    char keywords_t2D[KW_NO][STRLEN_MAX];
    size_t indices[KW_NO];

	if(TEST) printf("Wpisz nr testu ");
	scanf ("%d", &to_do);
	switch (to_do) {
    case 1:
      n_str_copy(keywords_t2D, keywords_ptab, KW_NO);
      ptab_sort(keywords_ptab, KW_NO);
      t2D_sort(keywords_t2D, indices, KW_NO);
      if(TEST) printf("Wpisz liczbe lancuchow n ");
      scanf("%d", &n);
      print_ptab(keywords_ptab, n);
      print_t2D_ind(keywords_t2D, indices, n);
      break;
    case 2:
      if(TEST) printf("Wpisz liczbe wierszy i liczbe kolumn macierzy 1: ");
      scanf("%d %d", &m, &n);
      if(TEST) printf("Wpisz elementy macierzy 1 (wierszami): ");
      read_mat(A,m,n);
      if(TEST) printf("Wpisz liczbe wierszy i liczbe kolumn macierzy 2: ");
      scanf("%d %d", &p, &q);
      if(TEST) printf("Wpisz elementy macierzy 2 (wierszami): ");
      read_mat(B,p,q);
      if(n == p) mat_product(A,B,C,m,n,q);
      print_mat(C,m,q);
      break;
    case 3:
      if(TEST) printf("Wpisz liczbe wierszy (rowna liczbie kolumn) macierzy kwadratowej: ");
      scanf("%d", &n);
      if(TEST) printf("Wpisz elementy macierzy (wierszami): ");
      read_mat(A,n,n);
      printf("%.4f\n", gauss_simplified(A, n));
      print_mat(A,n,n);
      break;
    default:
      printf("NOTHING TO DO FOR %d\n", to_do);
  }
  return 0;
}
