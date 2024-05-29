#include <stdio.h>
#include <graphviz/gvc.h>
#ifndef GRAPH_H
#define GRAPH_H


typedef struct Edge
{
    char *dest;
    size_t *ports;
    int ports_cnt;
    struct Edge *next;
    
    int in_ostov;
} Edge;

typedef struct Node
{
    char *name;
    size_t port;
    Edge *first_edge;
    struct Node *next;
    
    int color;
    
    int dist;
    struct Node *prev;
} Node;

typedef struct Graph
{
    Node *first_node;
    int size;
} Graph;

Graph *create_graph();
int add_node(Graph *, char *, size_t);
int add_edge(Graph *, char *, char *, size_t *, int);

Node *find_node(Graph *, char *);
Edge *find_edge(Node *, char *);

int del_node(Graph *, char *);
int del_edge(Graph *, char *, char *);

int clear_graph(Graph *);
int clear_edges(Node *);
int clear_edge(Edge *);
int clear_node(Node *);

int n_port_change(Graph *, char *, size_t);
int e_ports_change(Graph *, char *, char *, size_t *, int);
int output_graph(Graph *, int);

Node *BFS(Graph *, char *, size_t, int *);
Node *BFS_arr(Graph *, char *, size_t, int *);

Node **Bellman_Ford(Graph *, char *, char *, int *);
int ostov(Graph *);
int visual(Graph *, char *);
int paint_graph(Graph *, Agraph_t *);
#endif