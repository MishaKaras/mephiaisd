#ifndef F1_H
#define F1_H
struct Stack;
/*{
	int mx_size;
	int top;
	int *array;
} Stack;*/

struct Item;
/*{
	int data;
	struct Item *next;
} Item;*/

/*struct Stack_l;
{
	Item *top;
} Stack_l;*/

int push(struct Stack *, int);
int pop(struct Stack *, int *);
struct Stack *create();
void stack_clear(struct Stack *);
int make_empty(struct Stack *);
#endif
