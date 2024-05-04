#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "inputfuncs.h"
#include "bintree.h"
#include "dialogs.h"

int dialog();

int main()
{	
	int stat;

	int (*fptr[])(Tree *) = {NULL, D_insert, D_delete, D_find, D_output, D_traverse, D_minimum, D_import, D_dop, D_graphviz};
	int type = dialog();
	
	Tree *tree = create();
	while (type > 0)
	{
		stat = fptr[type](tree);
		if (stat == -1)
			break;
		type = dialog();
	}
	printf("Окончание программы.\n");
	if (tree->root != NULL)
		clear(tree);
	free(tree);
	return 0;
}

int dialog()
{
	char *msgs[] = {"0) Завершение;", "1) Вставка элемента;", "2) Удаление элемента;", "3) Поиск элемента;", "4) Вывод дерева;", "5) Обход дерева;", "6) Наименьший ключ;", "7) Импорт из файла;", "8) Доп. задание.", "9) Визуализация графа."};
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
	} while (res > 9 || res < 0);
	return res;
}
