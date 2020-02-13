#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "stack.h"

list *create_node(char ch, frspc **space)
{
	list *node = NULL;
	if (!*space)
	{
		node = malloc(sizeof(list));
		if (!node)
			return NULL;
	}
	else
	{
		node = (list *)(*space)->data;
		frspc *tmp = *space;
		(*space) = (*space)->next;
		free(tmp);
	}
	node->data = ch;
	node->next = NULL;
	return node;
}

list *create_node_t(char ch)
{
	list *node = NULL;
	node = malloc(sizeof(list));
	if (!node)
		return NULL;
	node->data = ch;
	node->next = NULL;
	return node;
}

void push(list **peak, int *n, list *node)
{
	if (!node)
		return;
	node->next = *peak;
	*peak = node;
	*n += 1;
}

list *pop(list **peak, int *n)
{
	if (!*peak)
		return NULL;
	list *node = *peak;
	(*peak) = (*peak)->next;
	*n -= 1;
	return node;
}

int is_palindrom(list **peak, int *n)
{
	int rc = 1;
	if (!*peak || *n == 0)
		return -1;
	
	list *tmp = NULL, *tmp1 = NULL, *tmp2 = NULL;
	int nt = 0, n1 = 0, n2 = 0;
	int m = *n / 2 + *n % 2;
	list *middle = NULL;
	while (*n > m)
		push(&tmp, &nt, pop(peak, n));
	if (m % 2 == 1)
		middle = pop(peak, n);
	
	while (*n > 0 && nt > 0 && rc == 1)
	{
		if ((*peak)->data != tmp->data)
			rc = 0;
		else
		push(&tmp1, &n1, pop(peak, n));
		push(&tmp2, &n2, pop(&tmp, &nt));
	}
	
	while (n1 > 0)
		push(peak, n, pop(&tmp1, &n1));
	push(peak, n, middle);
	while (nt > 0)
		push(peak, n, pop(&tmp, &nt));
	while (n2 > 0)
		push(peak, n, pop(&tmp2, &n2));
	return rc;
}

void print_list(list *peak, int *n, frspc *space)
{
	list *tmp = NULL;
	int ntmp = 0;
	if (n == 0 || !peak)
		printf("Stack is empty!");
	while (peak)
	{
		push(&tmp, &ntmp, pop(&peak, n));
		printf("%c\t%p\n", tmp->data, (void *)tmp);
	}
	printf("\n");
	printf("List of free spaces:\n");
	frspc *stmp = space;
	if (!stmp)
		printf("Empty\n");
	while (stmp)
	{
		printf("%p\n", stmp->data);
		stmp = stmp->next;
	}
	while (tmp)
		push(&peak, n, pop(&tmp, &ntmp));
}

void free_spaces(frspc **space)
{
	while (*space)
	{
		frspc *tmp = *space;
		(*space) = (*space)->next;
		free(tmp->data);
		free(tmp);
	}
}

void stack_free(list **peak, int *n)
{
	while (*peak)
	{
		list *node = *peak;
		(*peak) = (*peak)->next;
		free(node);
	}
	*n = 0;
}
