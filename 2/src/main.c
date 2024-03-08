#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>


#include "f1.h"
#include "inputfuncs.h"

int is_operator(char);
int is_operand(char);
char *str_input();
int func(char *, struct Stack *);
void print_error(int);

int main()
{
	char *inp = NULL;
	int stat = 0, res = 0;
	struct Stack *stack = create();
	do {
		inp = str_input();
		if (inp != NULL)
		{
			stat = func(inp, stack);
			if (stat == 0)
			{
				pop(stack, &res);
				stat = make_empty(stack);
				if (stat == 1)
					printf("Результат: %d\n", res);
				else
					printf("Неправильный формат выражения!\n");
			}
			else
			{
				print_error(stat);
				make_empty(stack);
			}
			free(inp);
		}
	} while (inp != NULL);
	stack_clear(stack);
	return 0;
}

void print_error(int stat)
{
	switch (stat)
	{
		case -1:
			printf("Ошибка: извлечение из пустого стека!\n");
			break;
		case -2:
			printf("Ошибка: запись в переполненный стек!\n");
			break;
		case -3:
			printf("Ошибка: попытка деления на ноль!\n");
			break;
		case -4:
			printf("Ошибка: введены некорректные данные!\n");
			break;
	}
}

char *str_input()
{
	int isgood = 1;
	char *str = NULL;
	do {
		isgood = 1;
		str = readline("Введите выражение в префиксной форме: ");
		if (str == NULL)
			return NULL;
		int len = strlen(str);
		if (len < 3 || !is_operator(str[0]) || is_operand(str[len-1]) == -1)
		{
			isgood = 0;
		}
		if (isgood)
			break;
		else
			free(str);
	} while (!isgood);
	return str;
}

int is_operator(char sym)
{
	int res = 0;
	switch (sym)
	{
		case '+':
			res = 1;
			break;
		case '-':
			res = 2;
			break;
		case '*':
			res = 3;
			break;
		case '/':
			res = 4;
			break;
		default:
			res = -1;
			break;
	}
	return res;
}

int is_operand(char sym)
{
	int res = -1;
	if (sym == ' ' || sym == '\t')
		return 0;
	for (int i = 0; i < 10; ++i) {
		if ((int)sym == 48 + i) {
			res = 1;
			break;
		}
		
	}
	return res;
}

int func(char *str, struct Stack *stack)
{
	int len = strlen(str), num = -1, stat = -1;
	for (int i = len - 1; i >= 0; --i)
	{
		char sym = str[i];
		if ( (stat = is_operand(sym)) == 1 )
		{
			num = (int)sym - 48;
			stat = push(stack, num);
			if (stat == -1)
				return -2; 
		}
		else if ( (stat = is_operator(sym)) != -1 )
		{
			int op1 = 0, op2 = 0, cur_res;
			if ( (pop(stack, &op1)) == -1)
				return -1; 
			if ( (pop(stack, &op2)) == -1)
				return -1; 
			switch (stat) {
				case 1:
					cur_res = op1 + op2;
					break;
				case 2:
					cur_res = op1 - op2;
					break;
				case 3:
					cur_res = op1 * op2;
					break;
				case 4:
					if (op2 == 0)
					{
						return -3;
					}
					cur_res = op1 / op2;
					break;
			}
			if ( (push(stack, cur_res)) == -1)
				return -2; 
		}
		else
		{
			if (sym == ' ' || sym == '\t')
				continue;
			return -4;
		}
	}
	return 0;
}
