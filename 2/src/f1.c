#include <stdio.h>
#include <stdlib.h>

#include "f1.h"
#include "inputfuncs.h"

typedef struct Item
{
	int data;
} Item;

typedef struct Stack
{
	int mx_size;
	int top;
	Item *array;
} Stack;


int push(Stack *stack, int elem)
{
	if (stack->top == stack->mx_size)
	{
		//printf("Отказ, стек полон.\n");
		stack->top = 0;
		return -1;
	}
	else
	{
		Item new;
		new.data = elem;
		stack->array[stack->top] = new;
		stack->top++;
	}
	return 0;
}

int pop(Stack *stack, int *elem)
{
	if (stack->top == 0)
	{
		//printf("Отказ, стек пуст.\n");
		free(stack->array);
		return -1;
	}
	else
	{
		stack->top--;
		*elem = stack->array[stack->top].data;
	}
	return 0;
}

Stack *create()
{
	int stat = 0, mx_elems = 0;
	Stack *new = (Stack*)malloc(sizeof(Stack));
	printf("Введите размер стека: ");
	stat = get_len(&mx_elems);
	if (stat == -1)
	{
		free(new);
		return NULL;
	}
	new->mx_size = mx_elems;
	new->top = 0;
	new->array = (Item*)malloc(mx_elems * sizeof(Item));
	return new;
}

void stack_clear(Stack* stack)
{
	if (stack->array != NULL)
		free(stack->array);
	free(stack);
}

int make_empty(Stack *stack)
{
	int res = 0;
	if (stack->top == 0)
		res = 1;
	else
		stack->top = 0;
	return res;
}
