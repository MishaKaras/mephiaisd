#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bintree.h"

char *key_gen();
int set_tree(Tree *tree, int size);
double insert_time(Tree *tree);
double del_time(Tree *tree);
double find_time(Tree *tree);


int main()
{
	Tree *tree = create();
	srand(time(NULL));
	FILE *fptr;
	//int size = 50000;
	double av_insert = 0.0, av_delete = 0.0, av_find = 0.0;
	double res = 0.0;
	fptr = fopen("insert.txt", "a");
	for (int size = 10000; size < 200001; size += 10000)
	{
		av_insert = 0.0;
		printf("size >> %d\n", size);
		for (int i = 0; i < 10; ++i)
		{
			set_tree(tree, size);
			res = insert_time(tree);
			av_insert += res;
			printf("insert-%d %lf\n", i+1, res);
			clear(tree);
			tree->root = NULL;
		}
		printf("1) Вставка элемента в дерево: %lf\n\n", av_insert / 10);
		fprintf(fptr, "%lf\n", av_insert);
	}
	fclose(fptr);
	
	fptr = fopen("delete.txt", "a");
	for (int size = 10000; size < 200001; size += 10000)
	{
		av_delete = 0.0;
		for (int i = 0; i < 10; ++i)
		{
			set_tree(tree, size);
			res = del_time(tree);
			av_delete += res;
			printf("delete-%d %lf\n", i+1, res);
			clear(tree);
			tree->root = NULL;
		}
		printf("2) Удаление элемента из дерева: %lf\n", av_delete / 10);
		fprintf(fptr, "%lf\n", av_delete);
	}
	fclose(fptr);
	
	fptr = fopen("find.txt", "a");
	for (int size = 10000; size < 200001; size += 10000)
	{
		av_find = 0.0;
		for (int i = 0; i < 10; ++i)
		{
			set_tree(tree, size);
			res = find_time(tree);
			av_find += res;
			printf("find-%d %lf\n", i+1, res);
			clear(tree);
			tree->root = NULL;
		}
		printf("3) Поиск элемента в дереве: %lf\n", av_find / 10);
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
		char *key = key_gen();
		size_t info = 1;
		insert(tree, key, info);
		free(key);
	}
	return 0;
}

char *key_gen()
{
	int len = rand() % 5 + 1;
	char *key = (char *)calloc(len + 1, sizeof(char));
	for (int i = 0; i < len; ++i)
	{
		key[i] = 'a' + rand() % 26;
	}
	return key;
}

double insert_time(Tree *tree)
{
	double total_time = 0.0;
	struct timespec start, end;
	for (int i = 0; i < 500; ++i)
	{
		char *key = key_gen();
		size_t info = 1;

		clock_gettime(CLOCK_REALTIME, &start);
		insert(tree, key, info);
		clock_gettime(CLOCK_REALTIME, &end);

		free(key);
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
		char *key = key_gen();

		clock_gettime(CLOCK_REALTIME, &start);
		delete(tree, key);
		clock_gettime(CLOCK_REALTIME, &end);

		free(key);
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
		char *key = key_gen();

		clock_gettime(CLOCK_REALTIME, &start);
		find_key(tree, key);
		clock_gettime(CLOCK_REALTIME, &end);

		free(key);
		total_time += (1000000000*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
	}
	return total_time / 500;
}
