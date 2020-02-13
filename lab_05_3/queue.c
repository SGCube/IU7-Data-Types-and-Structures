#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "error.h"

int push_queue(int val, list **pin, list **pout)
{
	list *node = malloc(sizeof(list));
	if (!node)
		return ERR_MEMORY;
	node->data = val;
	node->next = NULL;
	if (!*pin)
	{
		*pin = node;
		*pout = node;
	}
	else
	{
		(*pin)->next = node;
		*pin = (*pin)->next;
	}
	return OK;
}

int pop_queue(list **pin, list **pout)
{
	if (!*pout)
		return ERR_EMPTY;
	if (*pin == *pout)
		*pin = NULL;
	int rc = (*pout)->data;
	list *tmp = *pout;
	*pout = (*pout)->next;
	free(tmp);
	return rc;
}

void print_queue(list **pin, list **pout)
{
	if (!*pout)
	{
		printf("Queue is empty!\n");
		return;
	}
	
	list *tpin = NULL, *tpout = NULL;
	while (*pout)
	{
		void *addr = *pout;
		int tmp = pop_queue(pin, pout);
		printf("%d\t%p\n", tmp, addr);
		push_queue(tmp, &tpin, &tpout);
	}
	while (tpout)
		push_queue(pop_queue(&tpin, &tpout), pin, pout);
}
