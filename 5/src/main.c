#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "inputfuncs.h"
#include "bintree.h"
#include "dialogs.h"

#define EPS 0.00000001

int dialog();

int main()
{	
	int stat;
	double alpha = 0.0;
	char *s = "Введите коэффициент балансировки (в границах от 0.5 до 1): ";
	do
	{
		printf("%s", s);
		s = "Введите еще раз: ";
		stat = get_double(&alpha);
		if (stat == -1)
			return stat;
	} while (alpha > 1.0 || fabs(alpha - 1.0) < EPS || alpha < 0.5 || fabs(alpha - 0.5) < EPS);

	int (*fptr[])(Tree *) = {NULL, D_insert, D_delete, D_find, D_spec_find, D_output, D_traverse, D_import, D_graphviz};
	int type = dialog();
	
	Tree *tree = create(alpha);
	while (type > 0)
	{
		stat = fptr[type](tree);
		if (stat == -1)
			break;
		type = dialog();
	}
	printf("Окончание программы.\n");
	if (tree->root != NULL)
		clear(tree->root);
	free(tree);
	return 0;
}

int dialog()
{
	char *msgs[] = {"0) Завершение;", "1) Вставка элемента;", "2) Удаление элемента;", "3) Поиск элемента;", "4) Спец. поиск;", "5) Вывод дерева;", "6) Обход дерева;", "7) Импорт из файла;", "8) Визуализация графа.", "9) Доп. задание."};
	printf("----------------\n");
	for (int i = 0; i < 10; ++i)
		printf("%s\n", msgs[i]);
	printf("----------------\n\n");
	char *s = "Введите значение: ";
	int res = -1, stat = -1;
	do
	{
		printf("%s", s);
		s = "Введите еще раз: ";
		stat = get_int(&res);
		if (stat == -1)
			return stat;
	} while (res > 8 || res < 0);
	return res;
}
