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
    return new;
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
    new->color = 0;
    new->dist = 0;
    new->prev = NULL;
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

    free(edge->dest);
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
            if (is_ostov == 0 || (is_ostov == 1 && e_ptr->in_ostov == 1))
            {
                printf("\t-{ ");
                for (int i = 0; i < e_ptr->ports_cnt; ++i)
                    printf("%lu ", e_ptr->ports[i]);
                printf("}-> ");
                Node *dst = find_node(g, e_ptr->dest);
                printf("%7s | %lu\n", dst->name, dst->port);
            }
            e_ptr = e_ptr->next;
        }
        ptr = ptr->next;
    }
    printf("\n");
    return 0;
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

Node *BFS(Graph *g, char *start_name, size_t dest_port, int *stat)
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
        free(arr);
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
    {
        *way_ln = -1;
        return NULL;      // Не выделена память под граф
    }
    Node *start_node = find_node(g, start_name);
    Node *end_node = find_node(g, end_name);
    if (start_node == NULL)
    {
        *way_ln = -3;
        return NULL;      //Отсутствует вершина-источник
    }
    if (end_node == NULL)
    {
        *way_ln = -4;
        return NULL;      //Отсутствует вершина-цель
    }
    size_t dest_port = end_node->port;
    Node *ptr = g->first_node;
    Edge *e_ptr = NULL;
    Node *dst = NULL;
    while (ptr != NULL)
    {
        ptr->dist = INT_MAX;
        ptr->prev = NULL;
        ptr = ptr->next;
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

    Node **way = (Node **)calloc(g->size, sizeof(Node *));
    *way_ln = 0;
    ptr = end_node;
    while (ptr != NULL)
    {
        way[(*way_ln)++] = ptr;
        ptr = ptr->prev;
    }
    if (way[(*way_ln) - 1] != start_node)
    {
        *way_ln = -10;
        free(way);
        return NULL;
    }
    if ((*way_ln) < g->size)
    {
        Node **new_way = (Node **)realloc(way, (*way_ln) * sizeof(Node *));
        if (new_way == NULL)
        {
            free(way);
            *way_ln = -8;
            return NULL;
        }
        way = new_way;
    }
    return way;
}

int is_root(Graph *g, Node *node)
{
    if (node->first_edge == 0)
        return 0;
    Node *ptr = g->first_node;
    Edge *e_ptr = NULL;
    while (ptr != NULL)
    {
        if (ptr != node)
        {
            e_ptr = ptr->first_edge;
            while (e_ptr != NULL)
            {
                if (strcmp(e_ptr->dest, node->name) == 0)
                    return 0;
                e_ptr = e_ptr->next;
            }
        }
        ptr = ptr->next;
    }
    
    return 1;
}

int ostov(Graph *g, size_t o_port)
{
    if (g == NULL)
        return -1;      // Не выделена память под граф

    Node *ptr = g->first_node;
    Edge *e_ptr = NULL;
    Node **arr = (Node **)calloc(g->size, sizeof(Node *));
    int curr_arr_size = 0;
    while (ptr != NULL)
    {
        e_ptr = ptr->first_edge;
        while (e_ptr != NULL)
        {
            e_ptr->in_ostov = 0;        // Инициализация
            e_ptr = e_ptr->next;
        }
        if (ptr->port == o_port)
            arr[curr_arr_size++] = ptr;
        ptr = ptr->next;
    }
    if (curr_arr_size == 0)
    {
        free(arr);
        return -9;
    }
    arr = (Node **)realloc(arr, curr_arr_size * sizeof(Node *));
    ptr = g->first_node;
    int way_ln = 0;
    Node **way = NULL;
    while (ptr != NULL)
    {
        if (ptr->port == o_port)
        {
            ptr = ptr->next;
            continue;
        }
        for (int i = 0; i < curr_arr_size; ++i)
        {
            way = Bellman_Ford(g, ptr->name, arr[i]->name, &way_ln);

            if (way != NULL)
            {
                for (int j = way_ln - 1; j > 0; --j)
                {
                    e_ptr = way[j]->first_edge;
                    while (strcmp(e_ptr->dest, way[j-1]->name) != 0)
                        e_ptr = e_ptr->next;
                    e_ptr->in_ostov = 1;
                }
                free(way);
            }
        }
        ptr = ptr->next;
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
	int stat = paint_graph(graph, g);
    if (stat == -1)
        stat = -8;      // Ошибка выделения памяти
    else
    {
        gvLayout(gvc, g, "dot");
        gvRenderFilename(gvc, g, "svg", filename);
    }
	gvFreeLayout(gvc, g);
	agclose(g);
	gvFreeContext(gvc);
	return stat;
}

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
    res[0] = '{';
    for (int i = 0; i < ports_cnt; ++i)
    {
        char port_str[50];
        sprintf(port_str, "%lu", ports[i]);
        strcat(res, port_str);

        if (i < ports_cnt - 1) {
            strcat(res, ", "); 
        }
    }
    res[strlen(res)] = '}';
    char *res1 = realloc(res, (strlen(res) + 1) * sizeof(char));
    if (res1 == NULL)
    {
        free(res);
        return NULL;
    }
    res = res1;
    return res;
}
