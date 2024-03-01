#include <stdio.h>
#include <stdlib.h>
#include "test.h"

int main() {
    int** m = (int**) malloc(sizeof(int*) * 3);
    for (int i = 0; i < 3; ++i) {
        m[i] = (int*) malloc(sizeof(int) * 3);
        checkPtr(m[i]);
        for (int j = 0; j < 3; ++j) {
            m[i][j] = 0;
        }
        m[i][i] = i + 1;
    }
    printf("det = %d\n", determinant(m, 3));
    for (int i = 0; i < 3; ++i) {
        free(m[i]);
    }
    free(m);
    return 0;
}

int determinant(int** matrix, unsigned int size) {
    if (size == 1) {
        return matrix[0][0];
    }
    int det = 0;
    for (uint i = 0; i < size; ++i) {
        int** crossed = crossOut(matrix, size, i);
        det += ((i % 2) ? -1 : 1) * matrix[0][i] * determinant(crossed, size - 1);

        for (uint j = 0; j < size - 1; ++j) {
            free(crossed[j]);
        }
        free(crossed);
    }
    return det;
}

int** crossOut(int** matrix, uint size, uint x) {
    int** copy = (int**) malloc(sizeof(int*) * (size - 1));
    checkPtr(copy);
    for (uint i = 0; i < size - 1; ++i) {
        copy[i] = (int*) malloc(sizeof(int) * (size - 1));
        checkPtr(copy[i]);
        char xOffset = 0;
        for (uint j = 0; j < size - 1; ++j) {
            if (j == x) {
                xOffset = 1;
            }
            int v = matrix[i + 1][j + xOffset];
            copy[i][j] = v;
        }
    }
    return copy;
}

void checkPtr(void* ptr) {
    if (ptr == NULL) {
        exit(OUT_OF_MEMORY);
    }
}
