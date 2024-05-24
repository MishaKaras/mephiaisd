#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <graphviz/gvc.h>
#include <math.h>
#include "bintree.h"
#include "inputfuncs.h"

Node *find_prev(Tree *, Node *);
Node *rebalance(Node *);
int store_inorder(Node *, Node **, int);
Node *balanced_subtree(Node **, int, int, int/*, int*/);
int node_clear(Node *);
int clear_info_array(Node *);
int copy_info_array(Node *, Node *);
char *infos_in_str(Node *);


int traverse_rec(Node *ptr)
{
	if (ptr == NULL)
		return 0;
	traverse_rec(ptr->left);
	output_node(ptr);
	traverse_rec(ptr->right);
	return 0;
}

int output_node(Node *ptr)
{
	printf("Key >> %5lu\n", ptr->key);
	for (int i = 0; i < ptr->info_size; ++i)
		printf("\t%7s\n", (ptr->info_arr)[i]);
	return 0;
}

Node *find_next(Node *ptr)
{
	Node *par = ptr->parent;
	while (par != NULL && ptr == par->right)
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

Tree *create(double alpha)
{
	Tree *new = (Tree *)calloc(1, sizeof(Tree));
	new->size = 0;
	new->maxSize = 0;
	new->alpha = alpha;
	return new;
}

Node *new_node(const size_t key, const char *info)
{
	Node *new = (Node *)calloc(1, sizeof(Node));
	new->key = key;
	new->info_size = 1;
	new->info_arr = (char **)calloc(1, sizeof(char *));
	new->info_arr[0] = strdup(info);
	return new;
}

Node *find_parent(Tree *tree, const size_t key, int *depth)
{
	Node *ptr = tree->root;
	Node *par = NULL;
	int stat = 0;
	while (ptr != NULL)
	{
		stat = (int)key - (int)(ptr->key);
		par = ptr;
		if (stat < 0)
			ptr = ptr->left;
		else
			ptr = ptr->right;
		(*depth)++;
	}
	return par;	
}

Node *rebalance(Node *scapegoat)
{
	int goat_size = n_size(scapegoat);
	Node *inorder[goat_size];
	store_inorder(scapegoat, inorder, 0);
	Node *balanced = balanced_subtree(inorder, 0, goat_size - 1, scapegoat->depth);
	return balanced;
}

int store_inorder(Node *ptr, Node **arr, int index)
{
	if (ptr == NULL)
		return index;
	index = store_inorder(ptr->left, arr, index);
	arr[index++] = ptr;
	return store_inorder(ptr->right, arr, index);
}

Node *balanced_subtree(Node **inorder, int start, int end, int depth) 
{
	if (start > end)
		return NULL;
	int mid = (start + end) / 2;
	Node *ptr = inorder[mid];
	ptr->left = balanced_subtree(inorder, start, mid - 1, depth + 1);
	if (ptr->left != NULL)
		ptr->left->parent = ptr;
	
	ptr->right = balanced_subtree(inorder, mid + 1, end, depth + 1);
	if (ptr->right != NULL)
		ptr->right->parent = ptr;
	
	ptr->depth = depth;
	return ptr;
}

int insert(Tree *tree, const size_t key, const char *info)
{
	if (tree == NULL)
		return -1;		// Не выделена память под дерево
	Node *found = find_key(tree, key);
	if (found != NULL)
	{
		found->info_size++;
		char **new_arr = (char **)realloc(found->info_arr, found->info_size * sizeof(char *));
		if (new_arr != NULL)
			found->info_arr = new_arr;
		else
			return -1;
		(found->info_arr)[(found->info_size)-1] = strdup(info);
		return 0;
	}
	Node *inserted = b_insert(tree, key, info);
	tree->size++;
	if (tree->maxSize < tree->size)
		tree->maxSize = tree->size;
	
	if (inserted->depth > (int)(log(tree->size) / log(1 / tree->alpha)))
	{
		Node *goat = scapegoat(tree, inserted);
		Node *par = goat->parent;
		Node *res = rebalance(goat);
		res->parent = NULL;
		if (par != NULL)
		{
			if (goat == par->right)
				par->right = res;
			else
				par->left = res;
			res->parent = par;
		}
		else
			tree->root = res;

	}
	return 0;
}

Node *b_insert(Tree *tree, const size_t key, const char *info)
{
	int depth = 0;
	Node *new = new_node(key, info);
	if (tree->root == NULL)
		tree->root = new;
	else
	{
		Node *par = find_parent(tree, key, &depth);
		if (key < par->key)
		{
			par->left = new;
			new->parent = par;
		}
		else
		{
			par->right = new;
			new->parent = par;
		}
		new->parent = par;
	}
	new->depth = depth;
	return new;
}

Node *scapegoat(Tree *tree, Node *ptr)
{
	size_t size = 1, sib_size = 0;
	size_t par_size = 0;
	Node *par = ptr->parent;
	while (par != NULL)
	{
		if (ptr == par->right)
			if (par->left != NULL)
				sib_size = n_size(par->left);
			else
				sib_size = 0;
		else
			if (par->right != NULL)
				sib_size = n_size(par->right);
			else
				sib_size = 0;
		par_size = 1 + size + sib_size;
		if (size > (tree->alpha) * par_size || sib_size > (tree->alpha) * par_size)
			return par;
		ptr = par;
		par = par->parent;
		size = par_size;
	}
	return NULL;
}

int n_size(Node *ptr)
{
	if (ptr == NULL)
		return 0;
	return 1 + n_size(ptr->left) + n_size(ptr->right);
}

int delete(Tree *tree, const size_t key)
{
	int stat = b_delete(tree, key);
	if (stat < 0)
		return stat;
	
	tree->size--;
	if (tree->size < tree->alpha * tree->maxSize)
	{
		tree->root = rebalance(tree->root);
		tree->maxSize = tree->size;
	}
	return 0;
}

int b_delete(Tree *tree, const size_t del_key)
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
	}
	else
	{
		real_del = minimum(del_node->right);
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
		clear_info_array(del_node);
		copy_info_array(real_del, del_node);
		del_node->key = real_del->key;
	}
	node_clear(real_del);
	return 0;
}

int clear_info_array(Node *node)
{
	for (int i = 0; i < node->info_size; ++i)
		free((node->info_arr)[i]);
	free(node->info_arr);
	node->info_size = 0;
	return 0;
}

int copy_info_array(Node *src, Node *targ)
{
	targ->info_size = src->info_size;
	targ->info_arr = (char **)calloc(targ->info_size, sizeof(char *));
	for (int i = 0; i < targ->info_size; ++i)
		(targ->info_arr)[i] = strdup((src->info_arr)[i]);
	return 0;
	
}

Node *find_key(Tree *tree, const size_t f_key)
{
	if (tree == NULL)
		return NULL;

	Node *ptr = tree->root;
	while (ptr != NULL)
	{
		if (f_key == ptr->key)
			break;
		else if (f_key < ptr->key)
			ptr = ptr->left;
		else
			ptr = ptr->right;
	}
	return ptr;
}

Node *spec_find(Tree *tree, const size_t idol)
{
	Node *max = maximum(tree->root);
	Node *min = minimum(tree->root);
	if (((int)(max->key) - (int)idol) >= ((int)idol - (int)min->key))
		return find_key(tree, max->key);
	else
		return find_key(tree, min->key);
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
	printf("%lu >> ", ptr->key);
	for (int i = 0; i < ptr->info_size; ++i)
		printf("%7s | ", (ptr->info_arr)[i]);
	printf("\n");
	output(ptr->right, next_offset, 0, thread, spaces);
	output(ptr->left, next_offset, 0, thread, spaces);
	free(next_offset);
	return 0;
}

int clear(Node *root)
{
	if (root == NULL)
		return 0;
	clear(root->left);
	clear(root->right);
	node_clear(root);
	return 0;
}

int node_clear(Node *node)
{
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	clear_info_array(node);
	free(node);
	return 0;
}

int import(FILE *fptr, Tree *tree)
{
	int stat = 0;
	size_t key = 0;
	do
	{
		stat = fscanf(fptr, "%lu%*c", &key);
		if (stat != 1)
		{
			clear(tree->root);
			tree->root = NULL;
			return -5;		// Ошибка чтения числа из файла
		}
		char *info = txt_readline(fptr);
		if (info == NULL)
		{
			if (feof(fptr))
				return 0;
			clear(tree->root);
			tree->root = NULL;
			return -4;		// Ошибка чтения строки из файла
		}
		stat = insert(tree, key, info);
		free(info);
	} while (!feof(fptr));
	printf("Tree - size: %lu\n", tree->size);
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
	
	char *s_arr = infos_in_str(ptr);
	char name[500];
	sprintf(name, "<%lu>\n%s", ptr->key, s_arr);
	free(s_arr);
	Agnode_t *g_ptr = agnode(g, name, 1);
	
	if (ptr->left != NULL)
	{
		char l_name[500];
		char *l_s_arr = infos_in_str(ptr->left);
		sprintf(l_name, "<%lu>\n%s", ptr->left->key, l_s_arr);
		free(l_s_arr);
		Agnode_t *g_left = agnode(g, l_name, 1);
		agedge(g, g_ptr, g_left, 0, 1);
		addNode(ptr->left, g); 
	}
	if (ptr->right != NULL)
	{
		char r_name[500];
		char *r_s_arr = infos_in_str(ptr->right);
		sprintf(r_name, "<%lu>\n%s", ptr->right->key, r_s_arr);
		free(r_s_arr);
		Agnode_t *g_right = agnode(g, r_name, 1);
		agedge(g, g_ptr, g_right, 0, 1);
		addNode(ptr->right, g); 
	}
	return 0;
}


char *infos_in_str(Node *ptr)
{
	char *res = calloc(1, sizeof(char));
	int old_ln = 1;
	for (int i = 0; i < ptr->info_size; ++i)
	{
		if (res != NULL)
			old_ln = strlen(res) + 1;
		char *curr = (ptr->info_arr)[i];
		int c_ln = strlen(curr);
		char *new = realloc(res, (old_ln + c_ln + 1) * sizeof(char));
		if (new != NULL)
			res = new;
		else
			return NULL;
		strcat(res, curr);
		res[old_ln + c_ln - 1] = '\n';
		res[old_ln + c_ln] = '\0';
	}
	res[strlen(res)-1] = '\0';
	return res;
}