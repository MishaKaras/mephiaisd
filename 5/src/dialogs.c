#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "inputfuncs.h"
#include "dialogs.h"
#include "graph.h"

void errors(int stat)
{
	switch (stat)
	{
		case -1:
			printf("Ошибка. Граф пуст.\n");
			break;
		case -2:
			printf("Ошибка. Неуникальное имя вершины.\n");
			break;
		case -3:
			printf("Ошибка. Отсутствует вершина-источник.\n");
			break;
		case -4:
			printf("Ошибка. Отсутствует вершина-цель.\n");
			break;
		case -5:
			printf("Ошибка. Данное ребро уже присутствует.\n");
			break;
		case -6:
			printf("Ошибка. Вершина не найдена.\n");
			break;
		case -7:
			printf("Ошибка. Ребро не найдено.\n");
			break;
		case -8:
			printf("Ошибка выделения памяти.\n");
			break;
		case -9:
			printf("Ошибка. Отсутствует введенный порт.\n");
			break;
	}
}

int D_add_node(Graph *g)
{
	char *name = readline("Введите имя компьютера: ");
	if (name == NULL)
		return -1;
	printf("Введите порт подключения: ");
	size_t port;
	int stat = get_len(&port);
	if (stat == -1)
	{
		free(name);
		return -1;
	}
	stat = add_node(g, name, port);
	
	if (stat < 0)
		errors(stat);
	else
		printf("Вершина %s >> %lu добавлена.\n", name, port);
	free(name);
	return 0;
}

int D_add_edge(Graph *g)
{
	int stat = 0;
	char *src_name = readline("Введите имя источника: ");
	if (src_name == NULL)
		return -1;
	char *dst_name = readline("Введите имя цели: ");
	if (dst_name == NULL)
		return -1;

	printf("Введите количество портов:");
	char *s = " ";
	int ports_cnt = 0;
	do
	{
		printf("%s", s);
		s = "Введите еще раз: ";
		stat = get_int(&ports_cnt);
		if (stat == -1)
		{
			free(src_name);
			free(dst_name);
			return -1;
		}
	} while (ports_cnt <= 0);
	size_t *ports = (size_t *)calloc(ports_cnt, sizeof(size_t));
	for (int i = 0; i < ports_cnt; ++i)
	{
		size_t port = 0;
		printf("Ports[%d] = ", i);
		stat = get_len(&port);
		if (stat == -1)
		{
			free(ports);
			free(src_name);
			free(dst_name);
			return -1;
		}
		ports[i] = port;
	}
	stat = add_edge(g, src_name, dst_name, ports, ports_cnt);
	if (stat < 0)
		errors(stat);
	else
		printf("Ребро %s -> %s вставлено.\n", src_name, dst_name);
	free(src_name);
	free(dst_name);
	return 0;
}

int D_del_node(Graph *g)
{
	char *name = readline("Введите имя компьютера для удаления: ");
	if (name == NULL)
		return -1;
	
	int stat = del_node(g, name);
	if (stat < 0)
		errors(stat);
	else
		printf("Вершина %s удалена.\n", name);
	free(name);
	return 0;
}

int D_del_edge(Graph *g)
{
	int stat = 0;
	char *src_name = readline("Введите имя источника: ");
	if (src_name == NULL)
		return -1;
	char *dst_name = readline("Введите имя цели: ");
	if (dst_name == NULL)
		return -1;

	stat = del_edge(g, src_name, dst_name);
	if (stat < 0)
		errors(stat);
	else
		printf("Ребро %s -> %s удалено.\n", src_name, dst_name);
	free(src_name);
	free(dst_name);
	return 0;
}

int D_n_port_change(Graph *g)
{
	char *name = readline("Введите имя компьютера: ");
	if (name == NULL)
		return -1;
	printf("Введите новый порт: ");
	size_t port;
	int stat = get_len(&port);
	if (stat == -1)
	{
		free(name);
		return -1;
	}
	stat = n_port_change(g, name, port);
	if (stat < 0)
		errors(stat);
	else
		printf("Порт вершины %s изменен на %lu.\n", name, port);
	free(name);
	return 0;
}

int D_e_ports_change(Graph *g)
{
	int stat = 0;
	char *src_name = readline("Введите имя источника: ");
	if (src_name == NULL)
		return -1;
	char *dst_name = readline("Введите имя цели: ");
	if (dst_name == NULL)
		return -1;

	printf("Введите новое количество портов:");
	char *s = " ";
	int ports_cnt = 0;
	do
	{
		printf("%s", s);
		s = "Введите еще раз: ";
		stat = get_int(&ports_cnt);
		if (stat == -1)
		{
			free(src_name);
			free(dst_name);
			return -1;
		}
	} while (ports_cnt <= 0);
	size_t *ports = (size_t *)calloc(ports_cnt, sizeof(size_t));
	for (int i = 0; i < ports_cnt; ++i)
	{
		size_t port = 0;
		printf("New ports[%d] = ", i);
		stat = get_len(&port);
		if (stat == -1)
		{
			free(ports);
			free(src_name);
			free(dst_name);
			return -1;
		}
		ports[i] = port;
	}
	stat = e_ports_change(g, src_name, dst_name, ports, ports_cnt);
	if (stat < 0)
		errors(stat);
	else
		printf("Порты ребра %s -> %s изменены.\n", src_name, dst_name);
	free(src_name);
	free(dst_name);
	return 0;
}

int D_output(Graph *g)
{
	int stat = output_graph(g, 0);
	if (stat < 0)
		errors(stat);
	return 0;
}

int D_BFS(Graph *g)
{
	char *start_name = readline("Введите имя отсчетного компьютера: ");
	if (start_name == NULL)
		return -1;
	printf("Введите целевой порт: ");
	size_t dest_port = 0;
	int stat = get_len(&dest_port);
	if (stat == -1)
	{
		free(start_name);
		return -1;
	}
	stat = 0;
	Node *res = BFS(g, start_name, dest_port, &stat);
	if (res == NULL)
	{
		if (stat == -10)
			printf("Компьютер с портом %lu не найден.\n", dest_port);
		else
			errors(stat);
	}
	else
		printf("Ближайший к заданному %s компьютер с портом %lu >> %s\n",start_name, res->port, res->name);
	free(start_name);
	return 0;
}

int D_B_F(Graph *g)
{
	int way_ln = 0;
	char *start_name = readline("Введите имя источника: ");
	if (start_name == NULL)
		return -1;
	char *end_name = readline("Введите имя цели: ");
	if (end_name == NULL)
	{
		free(start_name);
		return -1;
	}

	Node **way = Bellman_Ford(g, start_name, end_name, &way_ln);
	if (way == NULL)
	{
		if (way_ln == -10)
			printf("Невозможно установить связь %s -> %s\n", start_name, end_name);
		else
			errors(way_ln);
	}
	else
	{
		printf("Кратчайший путь: \n");
		for (int i = way_ln - 1; i >= 0; --i)
		{
			Node *ptr = way[i];
			printf("%s | %lu ", ptr->name, ptr->port);
			if (i != 0)
				printf("-> ");
		}
		printf("\n");
		free(way);
	}
	free(start_name);
	free(end_name);
	return 0;
}

int D_ostov(Graph *g)
{
	size_t o_port = 0;
	printf("Введите порт для построения остова: ");
	int stat = get_len(&o_port);
	if (stat == -1)
		return -1; 
	stat = ostov(g, o_port);
	if (stat < 0)
		errors(stat);
	else
		output_graph(g, 1);
	return 0;
}

int D_graphviz(Graph *g)
{
	char *filename = readline("Введите имя svg-файла: ");
	if (filename == NULL)
		return -1;
	int stat = visual(g, filename);
	if (stat < 0)
		errors(stat);
	free(filename);
	return 0;
}
