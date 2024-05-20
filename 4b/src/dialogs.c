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
		case -4:
			printf("Ошибка чтения значения-строки из файла.\n");
			break;
		case -5:
			printf("Ошибка чтения ключа-числа из файла.\n");
			break;
		case -6:
			printf("Не удалось открыть файл.\n");
			break;
	}
}

int D_traverse(Tree *tree)
{
	int stat = 0;
	printf("\tОбход дерева\n");
	stat = traverse_rec(tree->root);
	if (stat < 0)
		errors(stat);
	return 0;
}

/*int D_minimum(Tree *tree)
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
}*/

int D_spec_find(Tree *tree)
{
	printf("Задайте значение: ");
	size_t idol = 0;
	int stat = get_len(&idol);
	if (stat == -1)
		return -1;

	Node *res = spec_find(tree, idol);
	printf("Результат поиска:\n");
	output_node(res);
	return 0;
}

int D_insert(Tree *tree)
{
	size_t key = 0; 
	int stat;
	char *info = NULL;

	printf("Введите ключ: ");
	stat = get_len(&key);
	if (stat == -1)
		return -1;

	info = readline("Введите информацию: ");
	if (info == NULL)
		return -1;
	
	stat = insert(tree, key, info);
	if (stat == -1)
		printf("Память под дерево не выделена.\n");
	else
		printf("Элемент %lu >> %s вставлен.\n", key, info);
	free(info);
	return 0;
}

int D_delete(Tree *tree)
{
	size_t key = 0;
	int stat;
	printf("Введите ключ: ");
	stat = get_len(&key);
	if (stat == -1)
		return -1;
	
	stat = delete(tree, key);
	if (stat < 0)
		errors(stat);
	else
		printf("Ключ %lu успешно удалён.\n\n", key);
	return 0;
}

int D_find(Tree *tree)
{
	size_t key;
	printf("Введите ключ: ");
	int stat = get_len(&key);
	if (stat == -1)
		return -1;
	Node *res = find_key(tree, key);
	if (res == NULL)
		errors(-2);
	else
		output_node(res);
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
		clear(tree->root);
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

// int D_dop(Tree *tree)
// {
// 	if (tree->root != NULL)
// 	{
// 		clear(tree);
// 		tree->root = NULL;
// 	}
// 	char *inp_name = readline("Введите название файла для чтения: ");
// 	if (inp_name == NULL)
// 		return -1;
// 	char *out_name = readline("Введите название файла для записи: ");
// 	if (out_name == NULL)
// 		return -1;

// 	int stat = dop(inp_name, out_name, tree);
// 	if (stat < 0)
// 		errors(stat);
// 	else
// 	{
// 		printf("Результат записан в файл %s.\n", out_name);
// 	}
// 	free(inp_name);
// 	free(out_name);
// 	return 0;
// }

int D_graphviz(Tree *tree)
{
	char *filename = readline("Введите название svg-файла для сохранения графа: ");
	int stat = visual(tree, filename);
	if (stat < 0)
		errors(stat);
	free(filename);
	return 0;
}
