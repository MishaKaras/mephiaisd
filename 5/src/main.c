#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "inputfuncs.h"
#include "graph.h"
#include "dialogs.h"

int dialog();

int main()
{	
	int stat;
	
	int (*fptr[])(Graph *) = {NULL, \
	D_add_node, D_add_edge, \
	D_del_node, D_del_edge, \
	D_n_port_change, D_e_ports_change, \
	D_output, \
	D_BFS, D_B_F, D_ostov, \
	D_graphviz};

	int type = dialog();
	
	Graph *g = create_graph();
	while (type > 0)
	{
		stat = fptr[type](g);
		if (stat == -1)
			break;
		type = dialog();
	}
	printf("Окончание программы.\n");
	clear_graph(g);
	return 0;
}

int dialog()
{
	char *msgs[] = {"0) Завершение;", \
	"1) Добавление компьютера;", \
	"2) Добавление канала связи;", \
	"3) Удаление компьютера;", \
	"4) Удаление канала связи;", \
	"5) Изменение порта подключения компьютера;", \
	"6) Изменение списка доступных портов канала связи;", \
	"7) Вывод сети;", \
	"8) Поиск ближайшего компьютера с заданным портом (BFS);", \
	"9) Поиск кратчайшего пути между заданными компьютерами (Беллман-Форд);", \
	"10) Минимизация сетевых подключений (построение остова);", \
	"11) Визуализация сети."};

	printf("----------------\n");
	for (int i = 0; i < 12; ++i)
		printf("%s\n", msgs[i]);
	printf("----------------\n\n");
	char *s = "Введите значение: ";
	int res = -1, stat = -1;
	do
	{
		printf("%s", s);
		s = "Введите еще раз: ";
		stat = get_int(&res);
		if (stat == -1)
			return stat;
	} while (res > 11 || res < 0);
	return res;
}
