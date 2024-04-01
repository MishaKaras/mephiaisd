#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inputfuncs.h"
#include "table.h"

unsigned set_release(Table *, unsigned);
int insert_release (Table *, char *, unsigned, unsigned);

Table *create ()
{
	Table *new = (Table*)calloc(1, sizeof(Table));
	new->ks = NULL;
	return new;
}

int insert (Table *table, char *info, const unsigned key)
{
	unsigned release = set_release(table, key);
	KeySpace *new = (KeySpace*)calloc(1, sizeof(KeySpace));
	new->key = key;
	new->release = release;
	new->info = info;
	new->next = table->ks;
	table->ks = new;
	return 0;
}

int insert_release (Table *table, char *info, const unsigned key, const unsigned release)
{
	
	KeySpace *new = (KeySpace*)calloc(1, sizeof(KeySpace));
	new->key = key;
	new->release = release;
	char *new_info = (char *)calloc(strlen(info) + 1, sizeof(char));
	new_info = strcpy(new_info, info);
	new->info = new_info;
	new->next = table->ks;
	table->ks = new;
	return 0;
}

unsigned set_release (Table *table, const unsigned find_key)
{
	unsigned res = 1;
	KeySpace *ptr = table->ks;
	while (ptr != NULL)
	{
		if (ptr->key == find_key && ptr->release >= res)
			res = ptr->release + 1;
		ptr = ptr->next;
	}
	return res;
}

int delete (Table *table, const unsigned del_key)
{
	if (table->ks == NULL)
		return -1;										// Таблица пуста
		
	int deleted = 0;
	KeySpace *rem = NULL;						
		
	KeySpace *ptr = table->ks;
	KeySpace *prev = NULL;
	while (ptr != NULL)
	{
		if (ptr->key == del_key)
		{
			deleted++;
			rem = ptr;
			if (ptr == table->ks)
				table->ks = table->ks->next;
			else
				prev->next = ptr->next;
			ptr = ptr->next;
			free(rem->info);
			free(rem);
			
		}
		else
		{
			prev = ptr;
			ptr = ptr->next;
		}
	}
	return deleted;
}

int delete_release (Table *table, const unsigned del_key, const unsigned del_release)
{
	if (table->ks == NULL)
		return -1; 										// Таблица пуста

	KeySpace *ptr = table->ks;
	KeySpace *prev = NULL;
	while (ptr != NULL && (ptr->key != del_key || ptr->release != del_release))
	{
		prev = ptr;
		ptr = ptr->next;
	}
	
	if (ptr == NULL)
		return -3;										// Отсутствует нужный релиз
	
	if (ptr == table->ks)								//Удаляется верхний элемент таблицы
		table->ks = table->ks->next;
	else
		prev->next = ptr->next;
	free(ptr->info);
	free(ptr);
	return 0;
}

int output (Table *table)
{
	if (table->ks == NULL)
		return -1;										// Таблица пуста
	KeySpace *ptr = table->ks;
	while (ptr != NULL)
	{
		printf("%5u | %5u : %s\n", ptr->key, ptr->release, ptr->info);
		ptr = ptr->next;
	}
	return 0;
}

int find (Table *table, const unsigned find_key, Table *res)
{
	if (table->ks == NULL)
		return -1;										// Таблица пуста

	KeySpace *ptr = table->ks;
	while (ptr != NULL)
	{
		if (ptr->key == find_key)
			insert_release(res, ptr->info, find_key, ptr->release);
		ptr = ptr->next;
	}
	if (res->ks == NULL)
		return -2;										// Нужный ключ не найден

	return 0;
}

int find_release (Table *table, const unsigned f_key, const unsigned f_release, Table *res)
{
	if (table->ks == NULL)
		return -1;										// Таблица пуста

	clear(res);
	KeySpace *ptr = table->ks;
	while (ptr != NULL && (ptr->key != f_key || ptr->release != f_release))
		ptr = ptr->next;
	if (ptr != NULL)
		insert_release(res, ptr->info, f_key, ptr->release);
	else
		return -3;										// Отсутствует нужный релиз
	return 0;
}

int import_txt (FILE *fptr, Table *new)
{
	clear(new);
	int key, stat;
	while (!feof(fptr))
	{
		stat = fscanf(fptr, "%d%*c", &key);
		if (!stat || key <= 0)
		{
			clear(new);
			return -5;									// Ошибка чтения ключа
		}
		//printf("Key: %d\n", key);

		char *info = txt_readline(fptr);
		//printf("FEOF: %d\n", feof(fptr));
		if (info == NULL)
		{
			if (feof(fptr))
				return 0;
			clear(new);
			return -4;									// Ошибка чтения строки из файла
		}
		//printf("Info: %s\n\n", info);
		insert(new, info, (unsigned)key);
	}
	return 0;
}

void clear (Table *table)
{
	KeySpace *ptr = NULL;
	while (table->ks != NULL)
	{
		ptr = table->ks;
		table->ks = table->ks->next;
		if (ptr->info != NULL)
			free(ptr->info);
		free(ptr);
	}
}
