#ifndef TEST_H_
#define TEST_H_

#define OUT_OF_MEMORY 11
#define uint unsigned int
void checkPtr(void* ptr);
int determinant(int** matrix, unsigned int size);
int** crossOut(int** matrix, uint size, uint x);
#endif // TEST_H_
