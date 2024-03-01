#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <readline/readline.h>

#include "matr.h"
#include "inputfuncs.h"

int bwrite_matrix(Matrix *);
void print_vector(FILE *fptr, long offset, int arrln);
int max_index(FILE *fptr, Matrix_b *matr_b);

int main()
{
	int index = -1, type = -1, stat = 0;
	printf("Выберите вариант работы программы:\n\
(1) Основной поток;\n\
(2) Бинарный файл;\n\
(3) Запись матрицы в бинарный файл.\n");
	do {
		printf(">> ");
		stat = get_len(&type);
		if (stat == -1)
	 		return 1;
	} while (type < 1 || type > 3);
	
 	if (type == 1) {
 		Matrix *matr = matrinp(&index);
		if (matr == NULL)
			return 1;
		printf("Входная матрица: \n");
		matrout(matr);
		printf("\n");
		printf("Индекс сортируемой строки: %d\n", index);
		
		Line *sortline = matr->lines + index;
		Line resline;
		resline.len = sortline->len;
		resline.arr = (int*)calloc(resline.len, sizeof(int));
		for (int i = 0; i < resline.len; ++i)
			resline.arr[i] = sortline->arr[i];
		qsort(resline.arr, resline.len, sizeof(int), compare);
		printf("Результат: \n");		
		for (int i = 0; i < resline.len; ++i)
			printf("%d ", resline.arr[i]);
		printf("\n");
		free_matrix(&matr);
		free(resline.arr);
		
	}
	else if (type == 2){
		int arrln = 0, elem = 0;
		char *filename = readline("Введите название файла для чтения: ");
		FILE *fptr = fopen(filename, "rb");
		if (fptr == NULL)
		{
			printf("Не удалось открыть файл.\n");
			return 1;
		}
		Matrix_b *matr_b = matr_b_read(fptr);

		index = max_index(fptr, matr_b);
		
		arrln = matr_b->lines[index].len;
		int catchid = 0, currmin = INT_MAX, previd = -1, prevmin = INT_MIN;


		char *filename_w = readline("Введите название файла для записи результата: ");
		FILE *fptr_w = fopen(filename_w, "w+b");
		if (fptr_w == NULL)
		{
			printf("Не удалось открыть файл.\n");
			return 1;
		}
		fwrite(&arrln, sizeof(int), 1, fptr_w);

		for (int j = 0; j < arrln; ++j) {
			fseek(fptr, matr_b->lines[index].offset, SEEK_SET);
			for (int i = 0; i < arrln; ++i)
			{
				fread(&elem, sizeof(int), 1, fptr);
				if (elem > prevmin || (elem == prevmin && i > previd))
				{
					if (elem < currmin)
					{
						currmin = elem;
						catchid = i;
					}
				}
			}
			printf("Elem [%d]: %d\n", j, currmin);
			fwrite(&currmin, sizeof(int), 1, fptr_w);
			prevmin = currmin;
			previd = catchid;
			currmin = INT_MAX;
		}
		fclose(fptr);
		free(filename);
		
		print_vector(fptr_w, sizeof(int), arrln);
		fclose(fptr_w);
		free(filename_w);
		
		free_matrix_b(&matr_b);
	}
	else
	{
		Matrix *matr = matrinp(&index);
		if (matr == NULL)
			return 1;
		printf("Входная матрица: \n");
		matrout(matr);
		printf("\n");
		bwrite_matrix(matr);
		free_matrix(&matr);
	}
	return 0;
}

int bwrite_matrix(Matrix *matr)
{
	char *filename_w = readline("Введите название файла для записи: ");
	FILE *fptr_w = fopen(filename_w, "wb");
	if (fptr_w == NULL)
	{
		printf("Не удалось открыть файл.\n");
		return 1;
	}
	fwrite(&(matr->len), sizeof(int), 1, fptr_w);
	long offset = sizeof(int) + matr->len * sizeof(Line_b);
	Line_b *w_line = (Line_b*)malloc(sizeof(Line_b));
	for (int i = 0; i < matr->len; ++i)
	{
		w_line->len = matr->lines[i].len;
		w_line->offset = offset;
		fwrite(w_line, sizeof(Line_b), 1, fptr_w);
		offset += matr->lines[i].len * sizeof(int);	
	}
	free(w_line);
	
	for (int i = 0; i < matr->len; ++i)
	{
		for (int j = 0; j < matr->lines[i].len; ++j)
		{
			fwrite(matr->lines[i].arr + j, sizeof(int), 1, fptr_w);
		}
	}
	fclose(fptr_w);
	free(filename_w);
	return 0;
}

void print_vector(FILE *fptr, long offset, int arrln)
{
	int elem = 0;
	fseek(fptr, offset, SEEK_SET);
	printf("Результирующий вектор: \n");
	for (int i = 0; i < arrln; ++i)
	{
		fread(&elem, sizeof(int), 1, fptr);
		printf("%d ", elem);
	}
	printf("\n");
}

int max_index(FILE *fptr, Matrix_b *matr_b)
{
	int currsum = 0, arrln = 0, elem = 0, mxsum = INT_MIN, index = -1;
	for (int i = 0; i < matr_b->len; ++i)
	{
		currsum = 0;
		arrln = matr_b->lines[i].len;
		fseek(fptr, matr_b->lines[i].offset, SEEK_SET);
		for (int j = 0; j < arrln; ++j)
		{
			fread(&elem, sizeof(int), 1, fptr);
			currsum += elem;
		}
		if (currsum > mxsum)
		{
			mxsum = currsum;
			index = i;
		}
	}
	return index;
}
