#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "matr.h"
#include "inputfuncs.h"

Matrix *matrinp(int *maxnum)
{
	int linescnt = 0, stat = 0, mxsum = 0, sum = 0;
	printf("Введите количество строк матрицы: ");
	stat = get_len(&linescnt);
	if (stat < 0)
		return NULL;
	Matrix *matr = (Matrix*)malloc(sizeof(Matrix));
	matr->len = linescnt;
	matr->lines = (Line*)malloc(linescnt * sizeof(Line));
	for (int i = 0; i < linescnt; ++i)
	{
		sum = 0;
		stat = lineinp(i, &(matr->lines[i]), &sum); 
		if (stat != 0)
		{
			for (int j = 0; j < i; ++j)
				free(matr->lines[j].arr);
			free(matr->lines);
			free(matr);
			return NULL;
		}
		if (sum > mxsum)
		{
			*maxnum = i;
			mxsum = sum;
		}
	}
	return matr;
}

int lineinp(int i, Line *line, int *sum)
{
	int stat = 0, lineln = 0, elem = 0;
	printf("Введите длину %d-й строки матрицы: ", i+1);
	stat = get_len(&lineln);
	if (stat < 0)
		return -1;
	line->len = lineln;
	line->arr = (int*)calloc(lineln, sizeof(int));
	for (int j = 0; j < lineln; ++j)
	{
		printf("Введите элемент %d-%d: ", i+1, j+1);
		stat = get_int(&elem);
		if (stat < 0)
		{
			free(line->arr);
			return -1;
		}
		line->arr[j] = elem;
		*sum += elem;
	}
	
	return 0;	
}


void matrout(Matrix *matr)
{
	for (int i = 0; i < matr->len; ++i)
	{
		Line line = matr->lines[i];
		for (int j = 0; j < line.len; ++j)
		{
			printf("%d ", (line.arr)[j]);
		}
		printf("\n");
	}
}

int compare(const void *v1, const void *v2)
{
	int *n1 = (int*)v1;
	int *n2 = (int*)v2;
	return (*n1) - (*n2);
}

void free_matrix(Matrix **matr)
{
	for (int i = 0; i < (*matr)->len; ++i)
		free((*matr)->lines[i].arr);
	free((*matr)->lines);
	free(*matr);
}

void free_matrix_b(Matrix_b **matr)
{
	free((*matr)->lines);
	free(*matr);
}

Matrix_b *matr_b_read(FILE *fptr)
{
	Matrix_b *matr_b = (Matrix_b*)malloc(sizeof(Matrix_b));
	fread(&matr_b->len, sizeof(int), 1, fptr);
	matr_b->lines = (Line_b*)malloc(matr_b->len * sizeof(Line_b));
	fread(matr_b->lines, sizeof(Line_b), matr_b->len, fptr);
	return matr_b;
}
