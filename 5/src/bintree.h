#include <stdio.h>
#include <graphviz/gvc.h>
#ifndef BINTREE_H
#define BINTREE_H
typedef struct Node
{
	size_t key;
	char **info_arr;
	int info_size;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
	int depth;
} Node;

typedef struct Tree
{
	Node *root;
	size_t size;
	size_t maxSize;
	double alpha;
} Tree;


Node *minimum(Node *);
Node *maximum(Node *);
Node *find_next(Node *);
int traverse(Tree *);
int traverse_rec(Node *);
int insert(Tree *, const size_t, const char *);
Node *b_insert(Tree *, const size_t, const char *);
int b_delete(Tree *, const size_t);
int delete(Tree *, const size_t);
Node *find_key(Tree *, const size_t);
Node *spec_find(Tree *, const size_t);
int output(Node *, char *, int, char *, char *);
int clear(Node *);
Tree *create(double);
int import(FILE *, Tree *);
int addNode(Node *, Agraph_t *);
int visual(Tree *, char *);
Node *scapegoat(Tree *, Node *);
int n_size(Node *);
int output_node(Node *);
#endif
