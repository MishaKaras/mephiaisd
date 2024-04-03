#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "inputfuncs.h"
#include "table.h"
#include "dialogs.h"

int dialog();

int main()
{	
	int (*fptr[])(Table *) = {NULL, D_insert, D_delete, D_delete_r, D_find, D_find_r, D_output, D_import};
	int type = dialog();
	int stat;
	Table *table = create();
	while (type > 0)
	{
		stat = fptr[type](table);
		if (stat == -1)
			break;
		type = dialog();
	}
	printf("Окончание программы.\n");
	clear(table);
	free(table);
	return 0;
}

int dialog()
{
	char *msgs[] = {"0) Завершение;", "1) Вставка элемента;", "2) Удаление по ключу;", "3) Удаление по релизу", "4) Поиск по ключу", "5) Поиск по релизу", "6) Вывод таблицы", "7) Импорт из файла .txt"};
	printf("----------------\n");
	for (int i = 0; i < 8; ++i)
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
	} while (res > 7 || res < 0);
	return res;
}
