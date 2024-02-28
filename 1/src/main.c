#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <readline/readline.h>

#include "matr.h"
#include "inputfuncs.h"

int bwrite_matrix(Matrix *);
void print_vector(FILE *fptr, long offset, int arrln);

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
		int arrln = 0, mxsum = INT_MIN, currsum = 0, elem = 0;
		long offset = 0;
		char *filename = readline("Введите название файла для чтения: ");
		FILE *fptr = fopen(filename, "a+b");
		if (fptr == NULL)
		{
			printf("Не удалось открыть файл.\n");
			return 1;
		}
		Matrix_b *matr_b = (Matrix_b*)malloc(sizeof(Matrix_b));
		fread(&matr_b->len, sizeof(int), 1, fptr);
		//printf("Lines: %d\n", matr_b->len);
		matr_b->lines = (Line_b*)malloc(matr_b->len * sizeof(Line_b));
		
		for (int i = 0; i < matr_b->len; ++i)
		{
			fread(&matr_b->lines[i].len, sizeof(int), 1, fptr);
			//printf("Len of line [%d]: %d\n", i, matr_b->lines[i].len);
			fread(&matr_b->lines[i].offset, sizeof(long), 1, fptr);
			//printf("Offset of line [%d]: %ld\n", i, matr_b->lines[i].offset);
		}
		
		for (int i = 0; i < matr_b->len; ++i)
		{
			currsum = 0;
			arrln = matr_b->lines[i].len;
			fseek(fptr, matr_b->lines[i].offset, SEEK_SET);
			//printf("Line-[%d] len: %d\n", i, arrln);
			for (int j = 0; j < arrln; ++j)
			{
				fread(&elem, sizeof(int), 1, fptr);
				//printf("elem[%d][%d]: %d\n", i, j, elem);
				currsum += elem;
				//printf("Curr position: %ld\n", ftell(fptr));		
			}
			if (currsum > mxsum)
			{
				mxsum = currsum;
				index = i;
			}
		}
		//printf("Offset: %ld\n", offset);
		//printf("mxsum: %d\n", mxsum);
		fseek(fptr, 0, SEEK_END);
		long res_offset = ftell(fptr);
		arrln = matr_b->lines[index].len;
		int catchid = 0, currmin = INT_MAX, previd = -1, prevmin = INT_MIN;
		//printf("Offset: %ld\n", matr_b->lines[index].offset);
		//printf("Arrln: %d\n", matr_b->lines[index].len);

		//Запись вектора 1
		offset = sizeof(int) + matr_b->len * (sizeof(int) + sizeof(long));
		fseek(fptr, offset, SEEK_SET);
		fwrite(&arrln, sizeof(int), 1, fptr);
		fwrite(&res_offset, sizeof(long), 1, fptr);


		//Запись вектора 2
		//fseek(fptr, 0, SEEK_END);
		//fwrite(&arrln, sizeof(int), 1, fptr);
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
			//printf("Elem [%d]: %d\n", j, currmin);
			fseek(fptr, 0, SEEK_END);
			fwrite(&currmin, sizeof(int), 1, fptr);
			prevmin = currmin;
			previd = catchid;
			currmin = INT_MAX;
		}
		offset = -1 * sizeof(int) * arrln;
		print_vector(fptr, offset, arrln);
		fclose(fptr);
		free(filename);
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
	long offset = sizeof(int) + matr->len * (sizeof(int) + sizeof(long));
	for (int i = 0; i < matr->len; ++i)
	{
		fwrite(&(matr->lines[i].len), sizeof(int), 1, fptr_w);
		fwrite(&(offset), sizeof(long), 1, fptr_w);
		offset += matr->lines[i].len * sizeof(int);	
	}
	
	for (int i = 0; i < matr->len; ++i)
	{
		for (int j = 0; j < matr->lines[i].len; ++j)
		{
			//printf("writing elem [%d][%d]: %d\n", i, j, *(matr->lines[i].arr + j));
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
	fseek(fptr, offset, SEEK_END);
	printf("Результирующий вектор: \n");
	for (int i = 0; i < arrln; ++i)
	{
		fread(&elem, sizeof(int), 1, fptr);
		printf("%d ", elem);
	}
	printf("\n");
}
