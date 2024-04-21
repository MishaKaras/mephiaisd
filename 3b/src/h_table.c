#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "inputfuncs.h"
#include "h_table.h"

#define GOLD (sqrt(5) - 1) / 2

int nodes_cnt(Node *);

unsigned h1(unsigned size, unsigned key)
{
	double hash = GOLD * key - (int)(GOLD * key);
	return (int)(hash * size);
}

unsigned h1_old(unsigned size, unsigned key)
{
	return key % size;
}

unsigned gcd(unsigned a, unsigned b)
{
	while (a > 0 && b > 0)
	{
		if (a >= b)
			a %= b;
		else
			b %= a;
	}
	return a + b;
}


unsigned h2(unsigned size, unsigned key)
{
	unsigned res = key % size;
	while (gcd(res, size) != 1)
		res++;
	return res;
}


Table *create(unsigned size)
{
	Table *new = (Table*)calloc(1, sizeof(Table));
	new->ks = (KeySpace*)calloc(size, sizeof(KeySpace));
	new->msize = size;
	new->csize = 0;
	return new;
}

unsigned mx_release(Node *nodes)
{
	Node *ptr = nodes;
	unsigned res = 0;
	while (ptr != NULL)
	{
		if (ptr->release > res)
			res = ptr->release;
		ptr = ptr->next;
	}
	return res;
}

int is_prime(unsigned num)
{
	if (num < 2 || num % 2 == 0)
		return num == 2;
	unsigned d = 3;
	while (d * d <= num && num % d != 0)
		d += 2;
	return (d * d) > num;
}

int resize(Table *table)
{
	unsigned new_size = table->msize * 2 + 1;
	unsigned new_pos = 0;
	while (is_prime(new_size) == 0)
		new_size += 2;
	KeySpace *new_ks = (KeySpace *)calloc(new_size, sizeof(KeySpace));
	for (unsigned i = 0; i < table->msize; ++i)
	{
		for (unsigned j = 0; j < new_size; ++j)
		{
			new_pos = h1(new_size, table->ks[i].key) + j * h2(new_size, table->ks[i].key);
			if (new_ks[new_pos].busy == 0)
				break;
		}
		new_ks[new_pos].busy = 1;
		new_ks[new_pos].key = table->ks[i].key;
		new_ks[new_pos].node = table->ks[i].node;
		table->ks[i].node = NULL;
	}
	free(table->ks);
	table->ks = new_ks;
	table->msize = new_size;
	return 0;
}


int insert(Table *table, char *data, unsigned key, unsigned release)
{
	int res = 0;
	Node *f_nodes = NULL;
	int found = find(table, key, &f_nodes);
	if (found == -2 && table->msize == table->csize)
		resize(table);
	clear_nodes(f_nodes);

	unsigned hash1 = h1(table->msize, key);
	unsigned hash2 = h2(table->msize, key);
	
	for (unsigned i = 0; i < table->msize; ++i)
	{
		unsigned hash = (hash1 + i * hash2) % table->msize;
		KeySpace *cur_ks = table->ks + hash;
		if (cur_ks->busy == 1)
		{
			if (cur_ks->key != key)
				continue;
			Node *new = (Node *)calloc(1, sizeof(Node));

			if (release == 0)
				new->release = mx_release(cur_ks->node) + 1;
			else
				new->release = release;
			new->info = data;
			new->next = cur_ks->node;
			
			cur_ks->node = new;
			res = 1;
			break;
		}
		else
		{
			if (cur_ks->busy == 0 && found == 1)
				continue;
			if (cur_ks->node != NULL)
				clear_nodes(cur_ks->node);
			cur_ks->busy = 1;
			cur_ks->key = key;
			Node *new = (Node *)calloc(1, sizeof(Node)); 
			if (release == 0)
				new->release = 1;
			else
				new->release = release;
			new->info = data;
			new->next = NULL;
			cur_ks->node = new;
			table->csize++;
			res = 1;
			break;
		}
	}
	return res;
}

int find(Table *table, unsigned f_key, Node **res_nodes)
{
	if (table->ks == NULL || table->csize == 0)
		return -1; // Таблица пуста
	
	unsigned hash1 = h1(table->msize, f_key);
	unsigned hash2 = h2(table->msize, f_key);
	for (unsigned i = 0; i < table->msize; ++i)
	{
		unsigned hash = (hash1 + i * hash2) % table->msize;
		KeySpace *cur_ks = table->ks + hash;
		if (cur_ks->busy == 1 && cur_ks->key == f_key)
		{
			Node *ptr = cur_ks->node;
			while (ptr != NULL)
			{
				Node *new = (Node *)calloc(1, sizeof(Node));
				new->release = ptr->release;
				new->info = strdup(ptr->info);
				new->next = *res_nodes;
				*res_nodes = new;
				ptr = ptr->next;
			}
			return 1;
		}
	}
	return -2; // Не удалось найти ключ 
}

int find_release (Table *table, unsigned f_key, unsigned f_release, char **res_info)
{
	if (table->ks == NULL || table->csize == 0)
		return -1; // Таблица пуста
	unsigned hash1 = h1(table->msize, f_key);
	unsigned hash2 = h2(table->msize, f_key);
	for (unsigned i = 0; i < table->msize; ++i)
	{
		unsigned hash = (hash1 + i * hash2) % table->msize;
		KeySpace *cur_ks = table->ks + hash;
		if (cur_ks->busy == 1 && cur_ks->key == f_key)
		{
			Node *ptr = cur_ks->node;
			while (ptr != NULL)
			{
				if (ptr->release == f_release)
				{
					if (*res_info != NULL)
						free(*res_info);
					*res_info = ptr->info;
					return 0;
				}
				ptr = ptr->next;
			}
			return -3; // Не удалось найти нужный релиз
		}
	}
	return -2; // Не удалось найти нужный ключ
}

int delete (Table *table, unsigned del_key)
{
	if (table->ks == NULL || table->csize == 0)
		return -1; // Таблица пуста
	unsigned hash1 = h1(table->msize, del_key);
	unsigned hash2 = h2(table->msize, del_key);
	for (unsigned i = 0; i < table->msize; ++i)
	{

		unsigned hash = (hash1 + i * hash2) % table->msize;
		KeySpace *cur_ks = table->ks + hash;
		if (cur_ks->key == del_key)
		{
			cur_ks->busy = 0;
			table->csize--;
			return 1;
		}
	}
	return -2; // Не удалось найти нужный ключ
}

int delete_release (Table *table, unsigned del_key, unsigned del_release)
{
	if (table->ks == NULL || table->csize == 0)
		return -1; // Таблица пуста
	unsigned hash1 = h1(table->msize, del_key);
	unsigned hash2 = h2(table->msize, del_key);
	for (unsigned i = 0; i < table->msize; ++i)
	{

		unsigned hash = (hash1 + i * hash2) % table->msize;
		KeySpace *cur_ks = table->ks + hash;
		if (cur_ks->key == del_key)
		{
			Node *ptr = cur_ks->node;
			Node *prev = NULL;
			while (ptr != NULL)
			{
				if (ptr->release == del_release)
				{
					if (prev == NULL)
					{
						cur_ks->node = ptr->next;
						free(ptr->info);
						free(ptr);
						if (cur_ks->node == NULL)
						{
							cur_ks->busy = 0;
							table->csize--;
						}
						return 1;
					}
					else
					{
						prev->next = ptr->next;
						free(ptr->info);
						free(ptr);
						return 1;
					}
				}
				prev = ptr;
				ptr = ptr->next;
			}
			return -3; // Не удалось найти нужный релиз
		}
	}
	return -2; // Не удалось найти нужный ключ
}

int output(Table *table)
{
	if (table->ks == NULL || table->csize == 0)
		return -1; //Таблица пуста
	printf("\n");
	printf("-----------------------------\n");
	for (unsigned i = 0; i < table->msize; ++i)
	{
		KeySpace cur_ks = table->ks[i];
		printf("Busy: %d | Key: %5u\n", cur_ks.busy, cur_ks.key);
		if (cur_ks.node == NULL)
			printf("\tСтрока пуста.\n");
		else
		{
			output_nodes(cur_ks.node);
		}
		printf("-----------------------------\n");
	}
	printf("\n");
	return 0;
}

int output_nodes(Node *nodes)
{
	if (nodes == NULL)
		return -1;
	Node *ptr = nodes;
	while (ptr != NULL)
	{
		printf("\t%5u : %s\n", ptr->release, ptr->info);
		ptr = ptr->next;
	}
	return 0;
}

int clear(Table *table)
{
	if(table->ks == NULL)
		return 0;
	for (unsigned i = 0; i < table->msize; ++i)
	{
		KeySpace cur_ks = table->ks[i];
		cur_ks.busy = 0;
		cur_ks.key = 0;
		clear_nodes(cur_ks.node);
	}
	if (table->ks != NULL)
		free(table->ks);
	return 0;
}

int clear_nodes(Node *nodes)
{
	if (nodes == NULL)
		return -1;
	Node *n_ptr = nodes;
	Node *n_prev = NULL;
	while (n_ptr != NULL)
	{
		n_prev = n_ptr;
		n_ptr = n_ptr->next;
		if (n_prev->info != NULL)
			free(n_prev->info);
		if (n_prev != NULL)
			free(n_prev);
	}
	return 0;
}

int import (FILE *fptr, Table *table)
{
	clear(table);
	unsigned msize = 0, csize = 0;
	fread(&msize, sizeof(unsigned), 1, fptr);
	fread(&csize, sizeof(unsigned), 1, fptr);
	if (csize > msize)
		return -4; // Ошибка чтения из файла
	table->msize = msize;
	table->csize = 0;
	table->ks = (KeySpace *)calloc(msize, sizeof(KeySpace));
	int n_cnt = 0, info_len = 0;
	unsigned key = 0, release = 0;
	for (unsigned i = 0; i < csize; ++i)
	{
		fread(&key, sizeof(unsigned), 1, fptr);
		fread(&n_cnt, sizeof(int), 1, fptr);
		for (int j = 0; j < n_cnt; ++j)
		{
			fread(&release, sizeof(unsigned), 1, fptr);
			fread(&info_len, sizeof(int), 1, fptr);
			char *info = (char *)calloc(info_len + 1, sizeof(char));
			fread(info, sizeof(char), info_len, fptr);
			insert(table, info, key, release);
		}
	}
	
	return 0;
}

int nodes_cnt (Node *nodes)
{
	Node *n_ptr = nodes;
	int res = 0;
	while (n_ptr != NULL)
	{
		++res;
		n_ptr = n_ptr->next;
	}
	return res;
}

int export(FILE *fptr, Table *table)
{
	
	if (table->ks == NULL || table->csize == 0)
		return -1; // Таблица пуста
	fwrite(&table->msize, sizeof(unsigned), 1, fptr);
	fwrite(&table->csize, sizeof(unsigned), 1, fptr);
	for (unsigned i = 0; i < table->msize; ++i)
	{
		KeySpace cur_ks = table->ks[i];
		if (cur_ks.busy == 1)
		{
			fwrite(&cur_ks.key, sizeof(unsigned), 1, fptr);

			int n_cnt = nodes_cnt(cur_ks.node);
			fwrite(&n_cnt, sizeof(int), 1, fptr);
			
			Node *n_ptr = cur_ks.node;
			while (n_ptr != NULL)
			{
				fwrite(&n_ptr->release, sizeof(unsigned), 1, fptr);
				int len = strlen(n_ptr->info);
				fwrite(&len, sizeof(int), 1, fptr);
				fwrite(n_ptr->info, sizeof(char), len, fptr);
				n_ptr = n_ptr->next;
			}
		}
	}
	return 0;
}
