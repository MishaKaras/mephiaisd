#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <graphviz/gvc.h>
#include "bintree.h"
#include "inputfuncs.h"

Node *find_prev(Tree *, Node *);

Node *first(Tree *tree)
{
	if (tree == NULL)
		return NULL;
	Node *ptr = tree->root;
	while (ptr->right != NULL)
		ptr = ptr->right;
	return ptr;
}

int traverse(Tree *tree, char *floor, char *ceil)
{
	printf("\t   Key   |  Info  \n");
	if (tree == NULL || tree->root == NULL)
		return -1;
	Node *ptr = first(tree);

	while (ptr != NULL)
	{
		printf("Curr: %8s\n", ptr->key);
		if (strcmp(ptr->key, floor) >= 0 && strcmp(ptr->key, ceil) <= 0)
			printf("\t%8s | %7lu\n", ptr->key, ptr->info);
		ptr = ptr->next;
	}
	return 0;
}

Node *find_next(Node *root)
{
	Node *ptr = root;
	Node *par = ptr->parent;
	while (par != NULL && ptr == par->left)
	{
		ptr = par;
		par = par->parent;
	}
	return par;
}

Node *minimum(Node *ptr)
{
	if (ptr == NULL)
		return NULL;
	while (ptr->left != NULL)
		ptr = ptr->left;
	return ptr;
}

Node *maximum(Node *ptr)
{
	if (!ptr)
		return NULL;
	while (ptr->right != NULL)
		ptr = ptr->right;
	return ptr;
}

Tree *create()
{
	Tree *new = (Tree *)calloc(1, sizeof(Tree));
	return new;
}

Node *new_node(const char *key, const size_t info)
{
	Node *new = (Node *)calloc(1, sizeof(Node));
	new->info = info;
	new->key = strdup(key);
	return new;
}

Node *find_parent(Tree *tree, const char *key)
{
	Node *ptr = tree->root;
	Node *par = NULL;
	int stat = 0;
	while (ptr != NULL)
	{
		stat = strcmp(key, ptr->key);
		par = ptr;
		if (stat < 0)
			ptr = ptr->left;
		else if (stat > 0)
			ptr = ptr->right;
		else
			return NULL;		// Дублирование ключей
	}
	return par;	
}

int insert(Tree *tree, const char *key, const size_t info)
{
	if (tree == NULL)
		return -1;		// Дерево пусто
	if (tree->root == NULL)
	{
		Node *new = new_node(key, info);
		tree->root = new;
	}
	else
	{
		Node *par = find_parent(tree, key);
		if (par == NULL)
			return -3;		//Дублирование ключей
		Node *new = new_node(key, info);
		if (strcmp(key, par->key) < 0)
		{
			par->left = new;
			par->next = new;
		}
		else
		{
			Node *par_prev = find_prev(tree, par);
			par->right = new;
			if (par_prev != NULL)
				par_prev->next = new;
		}
		new->parent = par;
		new->next = find_next(new);
	}
	return 0;
}

int node_clear(Node *node)
{
	node->parent = NULL;
	node->next = NULL;
	node->left = NULL;
	node->right = NULL;
	free(node->key);
	free(node);
	return 0;
}

Node *find_prev(Tree *tree, Node *curr_node)
{
	Node *prev = first(tree);
	if (prev == curr_node)
		return NULL;
	while (prev->next != NULL && prev->next != curr_node)
		prev = prev->next;
	return prev;
}

int delete(Tree *tree, const char *del_key)
{
	if (tree == NULL)
		return -1;		// Дерево отсутствует
	Node *del_node = find_key(tree, del_key);
	if (del_node == NULL)
		return -2;		// Удаляемый ключ отсутствует
		
	Node *real_del = NULL;
	Node *subtree = NULL;
	if (del_node->right == NULL || del_node->left == NULL)
	{
		real_del = del_node;
		Node *prev = find_prev(tree, del_node);		// Корректируем прошивку
		if (prev != NULL)
			prev->next = del_node->next;
	}
	else
	{
		real_del = minimum(del_node->right);

		Node *prev = find_prev(tree, real_del);		// Корректируем прошивку
		if (prev != NULL)
			prev->next = del_node;
	}
	
	if (real_del->right != NULL)
		subtree = real_del->right;
	else
		subtree = real_del->left;
	
	if (subtree != NULL)
		subtree->parent = real_del->parent;
	if (real_del->parent == NULL)
		tree->root = subtree;
	else
	{
		if (real_del == real_del->parent->left)
			real_del->parent->left = subtree;
		else
			real_del->parent->right = subtree;
	}
	if (del_node != real_del)
	{
		free(del_node->key);
		del_node->key = strdup(real_del->key);
		del_node->info = real_del->info;
	}
	node_clear(real_del);
	return 0;
}

Node *find_key(Tree *tree, const char *f_key)
{
	if (tree == NULL)
		return NULL;
	Node *ptr = tree->root;
	int compare = 0;
	while (ptr != NULL && (compare = strcmp(f_key, ptr->key)) != 0)
	{
		if (compare < 0)
			ptr = ptr->left;
		else
			ptr = ptr->right;
	}
	return ptr;
}


int output(Node *ptr, char *offset, int is_root, char *thread, char *spaces)
{
	if (ptr == NULL)
		return 0;
	char *next_offset = NULL;
	if (offset != NULL)
		next_offset = strdup(offset);
		
	if (!is_root)
	{
		if (offset != NULL)
		{
			printf("%s", offset);
			next_offset = (char*)realloc(next_offset, (strlen(offset) + 8) * sizeof(char));
		}
		else
			next_offset = (char*)calloc(8, sizeof(char));
			
		printf("+-");
		
		if (ptr == ptr->parent->right && ptr->parent->left)
			strcat(next_offset, thread);
		else
			strcat(next_offset, spaces);
		
	}
	printf("%s >> %lu\n", ptr->key, ptr->info);
	output(ptr->right, next_offset, 0, thread, spaces);
	output(ptr->left, next_offset, 0, thread, spaces);
	free(next_offset);
	return 0;
}

int clear(Tree *tree)
{
	if (tree == NULL)
		return 0;

	Node *prev = NULL;
	Node *ptr = first(tree);
	while (ptr != NULL)
	{
		prev = ptr;
		ptr = ptr->next;
		node_clear(prev);
	}
	tree->root = NULL;
	return 0;
}

int import(FILE *fptr, Tree *tree)
{
	int stat = 0;
	size_t info = 0;
	do
	{
		char *key = txt_readline(fptr);
		if (key == NULL)
		{
			if (feof(fptr))
				return 0;
			clear(tree);
			return -4;		// Ошибка чтения строки из файла
		}
		stat = fscanf(fptr, "%lu%*c", &info);
		if (stat != 1)
		{
			clear(tree);
			return -5;		// Ошибка чтения числа из файла
		}

		stat = insert(tree, key, info);
		if (stat == -3)
		{
			printf("Дублирование ключей, элемент %s пропущен.\n", key);
		}
		free(key);
	} while (!feof(fptr));

	return 0;
}

char *lower_case(char *word)
{
	char *lower = (char *)calloc(strlen(word) + 1, sizeof(char));
	for (size_t i = 0; i < strlen(word); ++i)
	{
		if (65 <= (int)word[i] && (int)word[i] <= 90)
		{
			int new = (int)word[i] + 32;
			lower[i] = (char)new;
		}
		else
			lower[i] = word[i];
	}
	return lower;
}

int is_letter(char s)
{
	return (65 <= (int)s && (int)s <= 90) || (97 <= (int)s && (int)s <= 122) || (48 <= (int)s && (int)s <= 57);
}

int write_dop(Tree *tree, FILE *fptr)
{
	if (tree->root == NULL)
		return 0;
	Node *ptr = first(tree);
	while (ptr != NULL)
	{
		fprintf(fptr, "%s - %lu\n", ptr->key, ptr->info);
		ptr = ptr->next;
	}
	return 0;
}

int dop(char *input_name, char *output_name, Tree *tree)
{
	FILE *fptr = fopen(input_name, "r");
	if (fptr == NULL)
		return -6;		//Не удалось открыть файл
	
	do
	{
		char *line = txt_readline(fptr);
		if (line == NULL)
			break;
		
		int word_len = 0;
		size_t line_len = strlen(line);
		char word[line_len];
		
		for (size_t i = 0; i < line_len; ++i)
		{
			if (is_letter(line[i]) || (line[i] == '-' && word_len > 0))
			{
				word[word_len] = line[i];
				word_len++;
			}
			else
			{
				if (word_len > 0)
				{
					word[word_len] = '\0';
					char *lower = lower_case(word);
					Node *found = find_key(tree, lower);
					if (found != NULL)
						found->info++;
					else
						insert(tree, lower, 1);
					word_len = 0;
					free(lower);
				}
			}
		}
		if (word_len > 0)
		{
			word[word_len] = '\0';
			char *lower = lower_case(word);
			Node *found = find_key(tree, lower);
			if (found != NULL)
				found->info++;
			else
				insert(tree, lower, 1);
			free(lower);
		}
		free(line);
	} while (!feof(fptr));
	fclose(fptr);

	FILE *fptr_w = fopen(output_name, "w");
	if (fptr_w == NULL)
		return -6;		//Не удалось открыть файл

	write_dop(tree, fptr_w);
	fclose(fptr_w);
	return 0;
}

int visual(Tree *tree, char *filename)
{
	if (tree == NULL || tree->root == NULL)
		return -1;
	
	GVC_t *gvc = gvContext();
	Agraph_t *g = agopen("BST", Agdirected, 0);
	addNode(tree->root, g);

	gvLayout(gvc, g, "dot");
	gvRenderFilename(gvc, g, "svg", filename);
	
	gvFreeLayout(gvc, g);
	agclose(g);
	gvFreeContext(gvc);
	return 0;
}

int addNode(Node *ptr, Agraph_t *g)
{
	if (ptr == NULL)
		return 0;
	
	char name[500];
	sprintf(name, "%s-%lu", ptr->key, ptr->info);
	Agnode_t *g_ptr = agnode(g, name, 1);
	
	if (ptr->next != NULL)
	{
		char next_name[500];
		sprintf(next_name, "%s-%lu", (ptr->next)->key, (ptr->next)->info);
		Agnode_t *g_next = agnode(g, next_name, 1);
		Agedge_t *n_edge = agedge(g, g_ptr, g_next, 0, 1);
		agsafeset(n_edge, "style", "dashed", "");
	}
	
	if (ptr->left != NULL)
	{
		char l_name[500];
		sprintf(l_name, "%s-%lu", (ptr->left)->key, (ptr->left)->info);
		Agnode_t *g_left = agnode(g, l_name, 1);
		/*Agedge_t *l_edge = */agedge(g, g_ptr, g_left, 0, 1);
		addNode(ptr->left, g); 
	}
	if (ptr->right != NULL)
	{
		char r_name[500];
		sprintf(r_name, "%s-%lu", (ptr->right)->key, (ptr->right)->info);
		Agnode_t *g_right = agnode(g, r_name, 1);
		/*Agedge_t *r_edge = */agedge(g, g_ptr, g_right, 0, 1);
		addNode(ptr->right, g); 
	}
	return 0;
}
