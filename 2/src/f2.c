#include <stdio.h>
#include <stdlib.h>

#include "f1.h"
#include "inputfuncs.h"

typedef struct Item
{
	int data;
	struct Item *next;
} Item;

typedef struct Stack
{
	Item *top;
} Stack;

int push(Stack *stack, int elem)
{
	Item *new = (Item*)malloc(sizeof(Item));
	new->data = elem;
	new->next = stack->top;
	stack->top = new;
	return 0;
}

int pop(Stack *stack, int *elem)
{
	if (stack->top == NULL)
	{
		//printf("Отказ, стек пуст.\n");
		return -1;
	}
	else
	{
		Item *x = stack->top;
		*elem = x->data;
		stack->top = x->next;
		x->next = NULL;
		free(x);
	}
	return 0;
}

Stack *create()
{
	Stack *new = (Stack*)malloc(sizeof(Stack));
	new->top = NULL;
	return new;
}

void stack_clear(Stack *stack)
{
	free(stack);
}

int make_empty(Stack *stack)
{
	int res = 0;
	if (stack->top == NULL)
		res = 1;
	else {
		Item *x = NULL;
		while (stack->top != NULL)
		{
			x = stack->top;
			stack->top = x->next;
			x->next = NULL;
			free(x);
		}
	}
	return res;
}
