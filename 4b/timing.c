#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "src/bintree.h"

int set_tree(Tree *tree, int size);
double insert_time(Tree *tree);
double del_time(Tree *tree);
double find_time(Tree *tree);


int main()
{
	srand(time(NULL));
	double alpha = 0.6;
	Tree *tree = create(alpha);
	double av_insert = 0.0, av_delete = 0.0, av_find = 0.0;
	double res = 0.0;

	FILE *fptr = fopen("insert.txt", "a");
	printf("Вставка\n");
	for (int size = 10000; size < 100001; size += 10000)
	{
		av_insert = 0.0;
		printf("size >> %d\n", size);
		for (int i = 0; i < 10; ++i)
		{
			set_tree(tree, size);
			res = insert_time(tree);
			av_insert += res;
			printf("insert-%d %lf\n", i+1, res);
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
	fptr = fopen("delete.txt", "a");
	for (int size = 100000; size < 3000001; size += 145000)
	{
		av_delete = 0.0;
		//printf("size >> %d\n", size);
		for (int i = 0; i < 10; ++i)
		{
			set_tree(tree, size);
			res = del_time(tree);
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
	fptr = fopen("find.txt", "a");
	for (int size = 100000; size < 3000001; size += 145000)
	{
		av_find = 0.0;
		for (int i = 0; i < 10; ++i)
		{
			set_tree(tree, size);
			res = find_time(tree);
			av_find += res;
			printf("find-%d %lf\n", i+1, res);
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
		size_t key = (size_t)rand() % 10000000;
		char *info = calloc(2, sizeof(char));
		info[0] = 'a';
		insert(tree, key, info);
	}
	return 0;
}

// char *key_gen()
// {
// 	int len = rand() % 5 + 1;
// 	char *key = (char *)calloc(len + 1, sizeof(char));
// 	for (int i = 0; i < len; ++i)
// 	{
// 		//char letter = (char)(rand() % 127 + 1);
// 		key[i] = 'a' + rand() % 26;
// 	}
// 	return key;
// }

double insert_time(Tree *tree)
{
	double total_time = 0.0;
	struct timespec start, end;
	for (int i = 0; i < 500; ++i)
	{
		size_t key = (size_t)rand();
		char *info = calloc(2, sizeof(char));
		info[0] = 'b';

		clock_gettime(CLOCK_REALTIME, &start);
		insert(tree, key, info);
		clock_gettime(CLOCK_REALTIME, &end);

		total_time += (1000000000*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
	}
	return total_time / 500;
}

double del_time(Tree *tree)
{
	double total_time = 0.0;
	struct timespec start, end;
	for (int i = 0; i < 500; ++i)
	{
		size_t key = (size_t)rand();
		clock_gettime(CLOCK_REALTIME, &start);
		delete(tree, key);
		clock_gettime(CLOCK_REALTIME, &end);
		
		total_time += (1000000000*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
	}
	return total_time / 500;
}

double find_time(Tree *tree)
{
	double total_time = 0.0;
	struct timespec start, end;
	for (int i = 0; i < 500; ++i)
	{
		size_t key = (size_t)rand();

		clock_gettime(CLOCK_REALTIME, &start);
		find_key(tree, key);
		clock_gettime(CLOCK_REALTIME, &end);

		total_time += (1000000000*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
	}
	return total_time / 500;
}
