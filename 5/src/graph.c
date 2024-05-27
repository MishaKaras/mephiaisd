#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <graphviz/gvc.h>
#include <limits.h>
#include "inputfuncs.h"
#include "graph.h"

char *write_ports(size_t *, int);
int in_arr(Node **, Node *, int);
int good_edge(size_t , Edge *);


Graph *create_graph()
{
    Graph *new = (Graph *)calloc(1, sizeof(Graph));
    new->first_node = NULL;
    new->size = 0;
}

int add_node(Graph *g, char *name, size_t port)
{
    if (g == NULL)
        return -1;      // Не выделена память под граф

    Node *ptr = g->first_node;
    Node *prev = NULL;
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, name) == 0)
            return -2;      // Неуникальный ключ
        prev = ptr;
        ptr = ptr->next;        
    }
    Node *new = (Node *)calloc(1, sizeof(Node));
    new->name = strdup(name);
    new->port = port;
    new->first_edge = NULL;
    new->next = NULL;
    if (prev != NULL)
        prev->next = new;
    else
        g->first_node = new;
    g->size++;
    return 0;
}

Node *find_node(Graph *g, char *f_name)
{
    if (g == NULL)
        return NULL;      // Не выделена память под граф
    
    Node *ptr = g->first_node;
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, f_name) == 0)
            break;
        ptr = ptr->next;
    }
    return ptr;
}

Edge *find_edge(Node *node, char *dst_name)
{    
    Edge *ptr = node->first_edge;
    while (ptr != NULL)
    {
        if (strcmp(ptr->dest, dst_name) == 0)
            break;
        ptr = ptr->next;
    }
    return ptr;
}

int add_edge(Graph *g, char *src_name, char *dst_name, size_t *ports, int p_cnt)
{
    if (g == NULL)
        return -1;      // Не выделена память под граф

    Node *src_node = find_node(g, src_name);
    if (src_node == NULL)
        return -3;      //Отсутствует вершина-источник

    Node *dst_node = find_node(g, dst_name);
    if (dst_node == NULL)
        return -4;      //Отсутствует вершина-цель
    
    Edge *ptr = src_node->first_edge;
    Edge *prev = NULL;
    while (ptr != NULL)
    {
        if (strcmp(ptr->dest, dst_name) == 0)
            return -5;      // Данное ребро уже присутствует
        prev = ptr;
        ptr = ptr->next;        
    }

    Edge *new = (Edge *)calloc(1, sizeof(Edge));
    new->dest = strdup(dst_name);
    new->ports = ports;
    new->ports_cnt = p_cnt;
    new->next = NULL;

    if (prev != NULL)
        prev->next = new;
    else
        src_node->first_edge = new;
    return 0;
}

int clear_graph(Graph *g)
{
    if (g == NULL)
        return 0;
    Node *ptr = g->first_node;
    Node *prev = NULL;
    while (ptr != NULL)
    {
        prev = ptr;
        ptr = ptr->next;
        clear_node(prev);
    }
    free(g);
    return 0;
}

int clear_node(Node *node)
{
    if (node == NULL)
        return 0;
    free(node->name);
    node->next = NULL;
    clear_edges(node);
    free(node);
    return 0;
}

int clear_edges(Node *node)
{
    Edge *ptr = node->first_edge;
    Edge *prev = NULL;
    while (ptr != NULL)
    {
        prev = ptr;
        ptr = ptr->next;
        clear_edge(prev);
    }
    node->first_edge = NULL;
    return 0;
}

int clear_edge(Edge *edge)
{
    if (edge == NULL)
        return 0;

    edge->dest = NULL;
    if (edge->ports != NULL)
        free(edge->ports);
    edge->next = NULL;
    free(edge);
    return 0;
}

int del_node(Graph *g, char *del_name)
{
    if (g == NULL)
        return -1;      // Не выделена память под граф

    Node *del_node = g->first_node;
    Node *prev = NULL;
    while (del_node != NULL)
    {
        if (strcmp(del_node->name, del_name) == 0)
            break;
        prev = del_node;
        del_node = del_node->next;        
    }
    if (del_node == NULL)
        return -6;      //Удаляемая вершина отсутствует
    
    if (prev != NULL)
        prev->next = del_node->next;
    else
        g->first_node = del_node->next;

    clear_node(del_node);
    g->size--;
    return 0;
}

int del_edge(Graph *g, char *src_name, char *dst_name)
{
    if (g == NULL)
        return -1;      // Не выделена память под граф

    Node *src_node = find_node(g, src_name);
    if (src_node == NULL)
        return -3;      //Отсутствует вершина-источник
    
    Edge *del_edge = src_node->first_edge;
    Edge *prev = NULL;
    while (del_edge != NULL)
    {
        if (strcmp(del_edge->dest, dst_name) == 0)
            break;
        prev = del_edge;
        del_edge = del_edge->next;
    }
    
    if (del_edge == NULL)
        return -7;      // Ребро отсутствует

    if (prev != NULL)
        prev->next = del_edge->next;
    else
        src_node->first_edge = del_edge->next;
    
    clear_edge(del_edge);
    return 0;
}

int n_port_change(Graph *g, char *name, size_t new_port)
{
    if (g == NULL)
        return -1;      // Не выделена память под граф

    Node *node = find_node(g, name);
    if (node == NULL)
        return -6;      //Изменяемая вершина отсутствует

    node->port = new_port;
    return 0;
}

int e_ports_change(Graph *g, char *src_name, char *dst_name, size_t *new_ports, int new_cnt)
{
    if (g == NULL)
        return -1;      // Не выделена память под граф
    
    Node *src_node = find_node(g, src_name);
    if (src_node == NULL)
        return -3;      //Отсутствует вершина-источник

    Edge *changind_edge = find_edge(src_node, dst_name);
    if (changind_edge == NULL)
        return -7;      // Ребро отсутствует

    if (changind_edge->ports != NULL)
        free(changind_edge->ports);

    changind_edge->ports = new_ports;
    changind_edge->ports_cnt = new_cnt;
    return 0;
}

int output_graph(Graph *g, int is_ostov)
{
    if (g == NULL)
        return -1;      // Не выделена память под граф
    Node *ptr = g->first_node;
    Edge *e_ptr = NULL;
    while (ptr != NULL)
    {
        printf("%7s | %lu\n", ptr->name, ptr->port);
        e_ptr = ptr->first_edge;
        while (e_ptr != NULL)
        {
            if (is_ostov == 0 || is_ostov == 1 && e_ptr->in_ostov == 1)
            {
                printf("\t-{ ");
                for (int i = 0; i < e_ptr->ports_cnt; ++i)
                    printf("%d ", e_ptr->ports[i]);
                printf("}-> ");
                Node *dst = find_node(g, e_ptr->dest);
                printf("%7s | %lu\n", dst->name, dst->port);
                e_ptr = e_ptr->next;
            }
        }
        ptr = ptr->next;
    }
    pritnf("\n");
    return 0;
}

int push(Queue *q, Node *node)
{
    Item *new = (Item *)calloc(1, sizeof(Item));
    new->node = node;
    new->next = NULL;
    if (q->tail != NULL)
        q->tail->next = new;
    else
        q->head = new;
    q->tail = new;        
    return 0;
}

Node *pop(Queue *q)
{
    if (q->head == NULL)
        return NULL;
    Node *res = q->head->node;
    Item *ptr = q->head;
    if (q->head == q->tail)
        q->tail = NULL;
    q->head = q->head->next;
    free(ptr);
    return res;
}

int q_clear(Queue *q)
{
    while (pop(q) != NULL) ;
    free(q);
    return 0;
}

Node *BFS(Graph *g, char *start_name, size_t dest_port, int *stat)
{
    if (g == NULL)
    {
        *stat = -1;
        return NULL;      // Не выделена память под граф
    }
    Queue *q = (Queue *)calloc(1, sizeof(Queue));
    Node *ptr = g->first_node;
    Edge *e_ptr = NULL;
    while (ptr != NULL)     
        ptr->color = 0;     // Инициализация
    ptr = find_node(g, start_name);
    if (ptr == NULL)
    {
        *stat = -3;
        return NULL;      //Отсутствует вершина-источник
    }
    ptr->color = 1;
    push(q, ptr);

    while (q->head != NULL)
    {
        ptr = pop(q);
        e_ptr = ptr->first_edge;
        while (e_ptr != NULL)
        {
            Node *adj_node = find_node(g, e_ptr->dest);
            if (adj_node->color == 0 && good_edge(dest_port, e_ptr))
            {
                if (adj_node->port == dest_port)
                {
                    q_clear(q);
                    return adj_node;
                }
                adj_node->color = 1;
                push(q, adj_node);
            }
            e_ptr = e_ptr->next; 
        }
    }
    *stat = -10;
    return NULL;
}

int in_arr(Node **arr, Node *curr, int arr_size)
{
    for (int i = 0; i < arr_size; ++i)
    {
        if (curr == arr[i])
            return 1;
    }
    return 0;
}

int good_edge(size_t port, Edge *edge)
{
    for (int i = 0; i < edge->ports_cnt; ++i)
    {
        if (port == edge->ports[i])
            return 1;
    }
    return 0;
}

Node *BFS_arr(Graph *g, char *start_name, size_t dest_port, int *stat)
{
    if (g == NULL)
    {
        *stat = -1;
        return NULL;      // Не выделена память под граф
    }
    int curr_size = 0, ptr_arr = 0;
    Edge *e_ptr = NULL;
    
    Node **arr = (Node **)calloc(g->size, sizeof(Node *));
    Node *ptr = find_node(g, start_name);
    if (ptr == NULL)
    {
        *stat =  -3;      //Отсутствует вершина-источник
        return NULL;
    }
    arr[curr_size++] = ptr;
    
    while (curr_size < g->size && ptr != NULL)
    {
        e_ptr = ptr->first_edge;
        while (e_ptr != NULL)
        {
            Node *adj_node = find_node(g, e_ptr->dest);

            if (in_arr(arr, adj_node, curr_size) || !good_edge(dest_port, e_ptr))
            {    
                e_ptr = e_ptr->next;
                continue;
            }
            if (adj_node->port == dest_port)
            {
                free(arr);
                return adj_node;
            }
            else
                arr[curr_size++] = adj_node;
            e_ptr = e_ptr->next;
        }
        ptr = arr[++ptr_arr];
    }
    if (arr != NULL)
        free(arr);
    *stat = -10;
    return NULL;
}

Node **Bellman_Ford(Graph *g, char *start_name, char *end_name, int *way_ln)
{
    if (g == NULL)
        return -1;      // Не выделена память под граф
    
    Node *start_node = find_node(g, start_name);
    Node *end_node = find_node(g, end_name);
    if (start_node == NULL)
        return -3;      //Отсутствует вершина-источник
    if (end_node == NULL)
        return -4;      //Отсутствует вершина-цель

    size_t dest_port = end_node->port;
    Node *ptr = g->first_node;
    Edge *e_ptr = NULL;
    Node *dst = NULL;
    while (ptr != NULL)
    {
        ptr->dist = INT_MAX;
        ptr->prev = NULL;
    }
    start_node->dist = 0;

    for (int i = 0; i < g->size - 1; ++i)
    {
        ptr = start_node;
        for (int j = 0; j < g->size; ++j)
        {
            if (ptr->dist != INT_MAX)
            {
                e_ptr = ptr->first_edge;
                while (e_ptr != NULL)
                {
                    if (good_edge(dest_port, e_ptr))
                    {
                        dst = find_node(g, e_ptr->dest);
                        if (dst->dist > ptr->dist + 1)
                        {
                            dst->dist = ptr->dist + 1;
                            dst->prev = ptr;
                        }
                    }
                    e_ptr = e_ptr->next;
                }
            }
            if (ptr->next != NULL)
                ptr = ptr->next;
            else
                ptr = g->first_node;
        }
    }

    // if (end_node->prev == NULL)
    //     return NULL;

    Node **way = (Node **)calloc(g->size, sizeof(Node *));
    *way_ln = 0;
    Node *ptr = end_node;
    while (ptr != NULL)
    {
        way[(*way_ln)++] = ptr;
        ptr = ptr->prev;
    }
    if (way[(*way_ln) - 1] != start_node)
    {
        free(way);
        return NULL;
    }
    if ((*way_ln) < g->size)
    {
        Node **new_way = (Node **)realloc(way, (*way_ln) * sizeof(Node *));
        if (new_way == NULL)
        {
            free(way);
            return NULL;
        }
        way = new_way;
    }
    return way;
}

int ostov(Graph *g)
{
    if (g == NULL)
        return NULL;      // Не выделена память под граф

    //Graph *res = (Graph *)calloc(1, sizeof(Graph));
    Node *ptr = g->first_node;
    Edge *e_ptr = NULL;
    while (ptr != NULL)
    {
        e_ptr = ptr->first_edge;
        while (e_ptr != NULL)
        {
            e_ptr->in_ostov = 0;
            e_ptr = e_ptr->next;
        }
        ptr = ptr->next;
    }

    //Edge **ostov_edges = NULL;
    Node **arr = (Node **)calloc(g->size, sizeof(Node *));
    int curr_size = 1;
    int arr_ptr = 0;
    ptr = g->first_node;
    arr[0] = ptr;
    while (curr_size < g->size && ptr != NULL)
    {
        e_ptr = ptr->first_edge;
        while (e_ptr != NULL)
        {
            Node *adj_node = find_node(g, e_ptr->dest);
            if (in_arr(arr, adj_node, curr_size) || !good_edge(adj_node->port, e_ptr))
            {    
                e_ptr = e_ptr->next;    
                continue;
            }
            e_ptr->in_ostov = 1;
            arr[curr_size++] = adj_node;
            e_ptr = e_ptr->next;
        }
        ptr = arr[++arr_ptr];
    }
    free(arr);
    return 0;
}


int visual(Graph *graph, char *filename)
{
	if (graph == NULL || graph->first_node == NULL)
		return -1;
	
	GVC_t *gvc = gvContext();
	Agraph_t *g = agopen("BST", Agdirected, 0);
	int stat = paint_graph_1(graph, g);
    if (stat == -1)
        return -8;      // Ошибка выделения памяти

	gvLayout(gvc, g, "dot");
	gvRenderFilename(gvc, g, "svg", filename);
	
	gvFreeLayout(gvc, g);
	agclose(g);
	gvFreeContext(gvc);
	return 0;
}

// int addNodes(Graph *graph, Agraph_t *g)
// {
//     Node *ptr = graph->first_node;
//     char name[500];
//     while (ptr != NULL)
//     {
//         sprintf(name, "%s\n%lu", ptr->name, ptr->port);
//         Agnode_t *g_ptr = agnode(g, name, 1);
//         ptr = ptr->next;
//     }
//     return 0;
// }

int paint_graph(Graph *graph, Agraph_t *g)
{
    Node *ptr = graph->first_node;
    Node *adj_node = NULL;
    Edge *e_ptr = NULL;
    while (ptr != NULL)
    {
        char name[500];
        sprintf(name, "%s\n%lu", ptr->name, ptr->port);
        Agnode_t *g_ptr = agnode(g, name, 1);

        e_ptr = ptr->first_edge;
        while (e_ptr != NULL)
        {
            char adj_name[500];
            adj_node = find_node(graph, e_ptr->dest);
            sprintf(adj_name, "%s\n%lu", adj_node->name, adj_node->port);
            Agnode_t *g_adj = agnode(g, adj_name, 1);
            Agedge_t *g_edge = agedge(g, g_ptr, g_adj, 0, 1);
            char *label = write_ports(e_ptr->ports, e_ptr->ports_cnt);
            if (label == NULL)
                return -1;
            agsafeset(g_edge, "label", label, "");
            if (e_ptr->in_ostov)
                agsafeset(g_edge, "style", "bold", "");
            free(label);
            e_ptr = e_ptr->next;
        }
        ptr = ptr->next;
    }
    return 0;
}

char *write_ports(size_t *ports, int ports_cnt)
{
    char *res = calloc(500, sizeof(char));
    res[0] = "{";
    int res_ln = 0;
    for (int i = 0; i < ports_cnt; ++i)
    {
        char port_str[50];
        sprintf(port_str, "%lu", ports[i]);
        strcat(res, port_str);

        if (i < ports_cnt - 1) {
            strcat(res, ", "); 
        }
    }
    res[strlen(res)] = "}";
    char *res1 = realloc(res, (strlen(res) + 1) * sizeof(char));
    if (res1 == NULL)
    {
        free(res);
        return NULL;
    }
    res = res1;
    return res;
}

// int addNode(Node *ptr, Agraph_t *g)
// {
// 	if (ptr == NULL)
// 		return 0;
	
// 	char *s_arr = infos_in_str(ptr);
// 	char name[500];
// 	sprintf(name, "<%lu>\n%s", ptr->key, s_arr);
// 	free(s_arr);
// 	Agnode_t *g_ptr = agnode(g, name, 1);
	
// 	if (ptr->left != NULL)
// 	{
// 		char l_name[500];
// 		char *l_s_arr = infos_in_str(ptr->left);
// 		sprintf(l_name, "<%lu>\n%s", ptr->left->key, l_s_arr);
// 		free(l_s_arr);
// 		Agnode_t *g_left = agnode(g, l_name, 1);
// 		agedge(g, g_ptr, g_left, 0, 1);
// 		addNode(ptr->left, g); 
// 	}
// 	if (ptr->right != NULL)
// 	{
// 		char r_name[500];
// 		char *r_s_arr = infos_in_str(ptr->right);
// 		sprintf(r_name, "<%lu>\n%s", ptr->right->key, r_s_arr);
// 		free(r_s_arr);
// 		Agnode_t *g_right = agnode(g, r_name, 1);
// 		agedge(g, g_ptr, g_right, 0, 1);
// 		addNode(ptr->right, g); 
// 	}
// 	return 0;
// }