#include <stdio.h>
#include <graphviz/gvc.h>
#ifndef BINTREE_H
#define BINTREE_H
typedef struct Node
{
	char *key;
	struct Node *left;
	struct Node *right;
	struct Node *next;
	struct Node *parent;
	size_t info; 
} Node;

typedef struct Tree
{
	Node *root;
} Tree;
Node *minimum(Node *);
Node *maximum(Node *);
Node *find_next(Node *);
int traverse(Tree *, char *, char *);
int insert(Tree *, const char *, const size_t );
int delete(Tree *, const char *);
int delete_2(Tree *, const char *);
Node *find_key(Tree *, const char *);
int output(Node *, char *, int, char *, char *);
int clear(Tree *);
Tree *create();
int import(FILE *, Tree *);
int dop(char *, char *, Tree *);
int addNode(Node *, Agraph_t *);
int visual(Tree *, char *);
#endif
