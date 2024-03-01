#include <stdio.h>
#ifndef MATR_H
#define MATR_H
typedef struct Line {
	int len;
	int *arr;
} Line;

typedef struct Matrix {
	int len;
	Line *lines;
} Matrix;

typedef struct Line_b {
	int len;
	long offset;
} Line_b;

typedef struct Matrix_b {
	int len;
	Line_b *lines;
} Matrix_b;

Matrix *matrinp(int *);
int lineinp(int, Line *, int *);
void matrout(Matrix *);
int compare(const void *, const void *);
void free_matrix(Matrix **);
void free_matrix_b(Matrix_b **);
Matrix_b *matr_b_read(FILE *);
#endif
