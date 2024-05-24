#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "src/bintree.h"

int set_tree(Tree *tree, int size);
double insert_time(Tree *tree, int);
double del_time(Tree *tree, int);
double find_time(Tree *tree, int);


int main()
{
	int sizes[] = {1000, 5000, 10000, 50000, 100000, 300000, 500000, 750000, 1000000, 1250000, 1500000, 1750000, 2000000, 2500000, 2750000, 3000000, 3500000, 3750000, 4000000, 4500000, 5000000};
	int sizes_cnt = 21;
	int size = 0;
	srand(time(NULL));
	double alpha = 0.6;
	Tree *tree = create(alpha);
	double av_insert = 0.0, av_delete = 0.0, av_find = 0.0;
	double res = 0.0;
	printf("Node*: %lu\n", sizeof(Node*));
	FILE *fptr = fopen("insert.txt", "w");
	printf("Вставка\n");
	for (int i = 0; i < sizes_cnt; i++)
	{
		size = sizes[i];
		av_insert = 0.0;
		printf("size >> %d\n", size);
		for (int i = 0; i < 10; ++i)
		{
			set_tree(tree, size);
			//printf("Дерево сгенерировано\n");
			res = insert_time(tree, size);
			av_insert += res;
			printf("\tinsert-%d %lf\n", i+1, res);
			clear(tree->root);
			tree->root = NULL;
		}
		av_insert /= 10;
		printf("\tВставка-%7d: %lf\n\n", size, av_insert);
		fprintf(fptr, "%lf\n", av_insert);
	}
	fclose(fptr);
	printf("--------------\n");

	printf("Удаление\n");
	fptr = fopen("delete.txt", "w");
	for (int i = 0; i < sizes_cnt; i++)
	{
		size = sizes[i];
		av_delete = 0.0;
		for (int i = 0; i < 10; ++i)
		{
			set_tree(tree, size);
			res = del_time(tree, size);
			av_delete += res;
			printf("\tdelete-%d %lf\n", i+1, res);
			clear(tree->root);
			tree->root = NULL;
		}
		av_delete /= 10;
		printf("Удаление-%7d: %lf\n", size, av_delete);
		fprintf(fptr, "%lf\n", av_delete);
	}
	fclose(fptr);
	printf("--------------\n");

	printf("Поиск\n");
	fptr = fopen("find.txt", "w");
	for (int i = 0; i < sizes_cnt; i++)
	{
		size = sizes[i];
		av_find = 0.0;
		for (int i = 0; i < 10; ++i)
		{
			set_tree(tree, size);
			res = find_time(tree, size);
			av_find += res;
			printf("\tfind-%d %lf\n", i+1, res);
			clear(tree->root);
			tree->root = NULL;
		}
		av_find /= 10;
		printf("Поиск-%7d: %lf\n", size, av_find);
		fprintf(fptr, "%lf\n", av_find);
	}
	fclose(fptr);
	free(tree);
	return 0;
}

int set_tree(Tree *tree, int size)
{
	for (int i = 0; i < size; ++i)
	{
		size_t key = (size_t)(rand() % size);
		char *info = (char *)calloc(2, sizeof(char));
		info[0] = 'a';
		insert(tree, key, info);
		free(info);
	}
	return 0;
}

double insert_time(Tree *tree, int size)
{
	double total_time = 0.0;
	struct timespec start, end;
	for (int i = 0; i < 100; ++i)
	{
		size_t key = (size_t)(rand() % size);
		char *info = calloc(2, sizeof(char));
		info[0] = 'b';

		clock_gettime(CLOCK_REALTIME, &start);
		insert(tree, key, info);
		clock_gettime(CLOCK_REALTIME, &end);
		free(info);
		total_time += (1000000000*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
	}
	return total_time/* / 100*/;
}

double del_time(Tree *tree, int size)
{
	double total_time = 0.0;
	struct timespec start, end;
	for (int i = 0; i < 100; ++i)
	{
		size_t key = (size_t)(rand() % size);
		clock_gettime(CLOCK_REALTIME, &start);
		delete(tree, key);
		clock_gettime(CLOCK_REALTIME, &end);
		
		total_time += (1000000000*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
	}
	return total_time/* / 100*/;
}

double find_time(Tree *tree, int size)
{
	double total_time = 0.0;
	struct timespec start, end;
	for (int i = 0; i < 100; ++i)
	{
		size_t key = (size_t)(rand() % UINT_MAX);

		clock_gettime(CLOCK_REALTIME, &start);
		find_key(tree, key);
		clock_gettime(CLOCK_REALTIME, &end);

		total_time += (1000000000*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
	}
	return total_time/* / 100*/;
}
