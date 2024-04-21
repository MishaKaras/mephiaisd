#include <stdio.h>
#ifndef H_TABLE
#define H_TABLE

typedef struct Node
{
	unsigned release;
	char *info;
	struct Node *next;
} Node;

typedef struct KeySpace
{
	int busy;
	unsigned key;
	Node *node;
} KeySpace;

typedef struct Table
{
	KeySpace *ks;
	unsigned msize;
	unsigned csize;
} Table;

Table *create (unsigned );
int insert (Table *, char *, unsigned , unsigned);
int find (Table *, unsigned , Node **);
int find_release (Table *, unsigned , unsigned , char **);
int delete (Table *, unsigned );
int delete_release (Table *, unsigned , unsigned );
int output(Table *);
int clear(Table *);
int import (FILE *, Table *);
int export(FILE *, Table *);
int output_nodes(Node *);
int clear_nodes(Node *);
#endif
