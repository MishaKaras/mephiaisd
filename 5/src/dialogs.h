#include "graph.h"
#ifndef DIALOGS_H
#define DIALOGS_H
void errors(int);
int D_add_node(Graph *g);
int D_add_adge(Graph *g);
int D_del_node(Graph *g);
int D_del_edge(Graph *g);
int D_n_port_change(Graph *g);
int D_e_ports_change(Graph *g);
int D_output(Graph *g);
int D_BFS(Graph *g);
//int D_graphviz(Graph *g);
#endif
