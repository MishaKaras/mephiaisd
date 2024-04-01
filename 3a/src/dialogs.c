#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "inputfuncs.h"
#include "table.h"
#include "dialogs.h"

void errors(int stat)
{
	switch (stat)
	{
		case -1:
			printf("Ошибка. Таблица пуста.\n");
			break;
		case -2:
			printf("Ошибка. Отсутствует запрашиваемый ключ элемента.\n");
			break;
		case -3:
			printf("Ошибка. Отсутствует запрашиваемый релиз элемента.\n");
			break;
		case -4:
			printf("Ошибка чтения СТРОКИ из текстового файла.\n");
			break;
		case -5:
			printf("Ошибка чтения КЛЮЧА из текстового файла.\n");
			break;
	}
}

int D_insert(Table *table)
{
	unsigned key; 
	int stat;
	char *info = NULL;
	printf("Введите ключ вставляемого элемента: ");
	stat = get_len(&key);
	if (stat == -1)
		return -1;
	info = readline("Введите информацию: ");
	if (info == NULL)
		return -1;
	
	insert(table, info, key);
	return 0;
}

int D_delete(Table *table)
{
	unsigned key;
	int stat;
	printf("Введите ключ удаляемого элемента: ");
	stat = get_len(&key);
	if (stat == -1)
		return -1;
	stat = delete(table, key);
	if (stat < 0)
		errors(stat);
	else
		printf("Было удалено %d элементов\n\n", stat);
	return 0;
}

int D_delete_r(Table *table)
{
	unsigned key, release;
	int stat;
	printf("Введите ключ удаляемого элемента: ");
	stat = get_len(&key);
	if (stat == -1)
		return -1;
	printf("Введите релиз удаляемого элемента: ");
	stat = get_len(&release);
	if (stat == -1)
		return -1;
		
	stat = delete_release(table, key, release);
	if (stat < 0)
		errors(stat);
	else
		printf("Элемент %5u | %5u успешно удален.\n\n", key, release);
	return 0;
}

int D_find(Table *table)
{
	unsigned key;
	int stat;
	printf("Введите ключ поиска: ");
	stat = get_len(&key);
	if (stat == -1)
		return -1;
	Table *found = create();
	stat = find(table, key, found);
	if (stat < 0)
		errors(stat);
	else
	{
		printf("Таблица найденных элементов: \n");
		output(found);
		clear(found);
	}
	free(found);
	return 0;
}

int D_find_r(Table *table)
{
	unsigned key, release;
	int stat;
	
	printf("Введите ключ поиска: ");
	stat = get_len(&key);
	if (stat == -1)
		return -1;
	printf("Введите релиз поиска: ");
	stat = get_len(&release);
	if (stat == -1)
		return -1;

	
	Table *found = create();
	stat = find_release(table, key, release, found);
	if (stat < 0)
		errors(stat);
	else
	{
		printf("Найденный элемент: \n");
		output(found);
		clear(found);
	}
	free(found);
	return 0;
}

int D_output(Table *table)
{
	int stat = output(table);
	if (stat < 0)
		errors(stat);
	return 0;
}

int D_import(Table *table)
{
	int stat;
	char *filename = readline("Введите название файла для чтения: ");
	if (filename == NULL)
		return -1;
	FILE *fptr = fopen(filename, "r");
	stat = import_txt(fptr, table);
	fclose(fptr);
	if (stat < 0)
		errors(stat);
	else
	{
		printf("Считанная таблица: \n");
		output(table);
	}
	free(filename);
	return 0;
}
