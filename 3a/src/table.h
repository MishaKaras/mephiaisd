#include <stdio.h>
#ifndef TABLE_H
#define TABLE_H
typedef struct KeySpace 
{
	unsigned key;
	unsigned release;
	char *info;
	struct KeySpace *next;
} KeySpace;

typedef struct Table
{
	KeySpace *ks;
} Table;

Table *create();
int insert (Table *, char *, const unsigned);
int delete (Table *, const unsigned);
int delete_release (Table *, const unsigned, const unsigned);
int output (Table *);
int find (Table *, const unsigned, Table *);
int find_release (Table *, const unsigned, const unsigned, Table *);
int import_txt (FILE *, Table *);
void clear (Table *);
#endif
