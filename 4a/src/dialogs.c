#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "inputfuncs.h"
#include "bintree.h"
#include "dialogs.h"

void errors(int stat)
{
	switch (stat)
	{
		case -1:
			printf("Ошибка. Дерево пусто.\n");
			break;
		case -2:
			printf("Ошибка. Отсутствует запрашиваемый ключ элемента.\n");
			break;
		case -3:
			printf("Ошибка. Дублирование ключей.\n");
			break;
		case -4:
			printf("Ошибка чтения ключа-строки из файла.\n");
			break;
		case -5:
			printf("Ошибка чтения значения-числа из файла.\n");
			break;
		case -6:
			printf("Не удалось открыть файл.\n");
			break;
	}
}

int D_traverse(Tree *tree)
{
	int stat = 0;
	char *floor = readline("Введите нижнюю границу ключей: ");
	if (floor == NULL)
		return 1;
	char *ceil = readline("Введите верхнюю границу ключей: ");
	if (ceil == NULL)
		return 1;
	stat = traverse(tree, floor, ceil);
	if (stat < 0)
		errors(stat);
	free(floor);
	free(ceil);
	return 0;
}

int D_minimum(Tree *tree)
{
	Node *res = minimum(tree->root);
	if (res == NULL)
	{
		errors(-1);
		return 1;
	}
	printf("Минимальный элемент дерева: \n");
	printf("\t%s >> %lu\n", res->key, res->info);
	return 0;
}

int D_insert(Tree *tree)
{
	char *key = NULL; 
	int stat;
	size_t info;
	key = readline("Введите ключ: ");
	
	if (key == NULL)
		return -1;
	
	printf("Введите информацию: ");
	stat = get_len(&info);
	if (stat == -1)
		return -1;
	
	stat = insert(tree, key, info);
	if (stat == -1)
	{
		printf("Память под дерево не выделена.\n");
	}
	free(key);
	return 0;
}

int D_delete(Tree *tree)
{
	char *key = NULL;
	int stat;
	key = readline("Введите ключ: ");
	if (key == NULL)
		return -1;
	stat = delete(tree, key);
	if (stat < 0)
		errors(stat);
	else
		printf("Ключ %s успешно удалён.\n\n", key);
	if (key != NULL)
		free(key);
	return 0;
}

int D_find(Tree *tree)
{
	char *key = NULL;
	key = readline("Введите ключ: ");
	if (key == NULL)
		return -1;
	Node *res = find_key(tree, key);
	if (res == NULL)
		errors(-2);
	else
		printf("\t%s >> %lu\n", res->key, res->info);
	free(key);
	return 0;
}


int D_output(Tree *tree)
{
	char *thread = "|    ";
	char *spaces = "     ";
	printf("\n-----------------------\n");
	output(tree->root, NULL, 1, thread, spaces);
	printf("\n-----------------------\n");
	return 0;
}

int D_import(Tree *tree)
{
	if (tree->root != NULL)
	{
		clear(tree);
		tree->root = NULL;
	}
	int stat = 0;
	char *filename = readline("Введите название файла для чтения: ");
	if (filename == NULL)
		return -1;
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL)
	{
		errors(-6);
		return -1;
	}
	stat = import(fptr, tree);
	fclose(fptr);
	if (stat < 0)
		errors(stat);
	else
		D_output(tree);
	free(filename);
	return 0;
}

int D_dop(Tree *tree)
{
	if (tree->root != NULL)
	{
		clear(tree);
		tree->root = NULL;
	}
	char *inp_name = readline("Введите название файла для чтения: ");
	if (inp_name == NULL)
		return -1;
	char *out_name = readline("Введите название файла для записи: ");
	if (out_name == NULL)
		return -1;

	int stat = dop(inp_name, out_name, tree);
	if (stat < 0)
		errors(stat);
	else
	{
		printf("Результат записан в файл %s.\n", out_name);
	}
	free(inp_name);
	free(out_name);
	return 0;
}
