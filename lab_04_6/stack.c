#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "stack.h"

int push(list **peak, char ch, frspc **space)
{
	list *elem = NULL;
	if (!*space)
	{
		elem = malloc(sizeof(list));
		if (!elem)
			return ERR_ALLOC;
	}
	else
	{
		elem = (list *)(*space)->data;
		frspc *tmp = *space;
		(*space) = (*space)->next;
		free(tmp);
	}
	elem->data = ch;
	elem->next = *peak;
	*peak = elem;
	return OK;
}

int pop(list **peak, frspc **space)
{
	frspc *tmp = malloc(sizeof(frspc));
	if (!tmp)
		return ERR_ALLOC;
	tmp->next = *space;
	*space = tmp;
	(*space)->data = (void *)(*peak);
	(*peak) = (*peak)->next;
	return OK;
}

int push_new(list **peak, char ch)
{
	list *elem = NULL;
	elem = malloc(sizeof(list));
	if (!elem)
		return ERR_ALLOC;
	elem->data = ch;
	elem->next = *peak;
	*peak = elem;
	return OK;
}

void pop_free(list **peak)
{
	list *tmp = *peak;
	(*peak) = (*peak)->next;
	free(tmp);
}

int peek(list *peak)
{
	list *tmp = peak;
	int len = 0;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	return len;
}

int is_palindrom(list *peak)
{
	int len = peek(peak);
	if (len == 0)
		return -1;
	list *tmp1 = peak;
	for (int k = 0; k < len / 2; k++)
	{
		list *tmp2 = tmp1;
		for (int j = k; j < len - k - 1; j++)
			tmp2 = tmp2->next;
		if (tmp1->data != tmp2->data)
			return 0;
		tmp1 = tmp1->next;
	}
	return 1;
}

void print_list(list *peak, frspc *space)
{
	list *tmp = peak;
	if (!tmp)
		printf("Stack is empty!");
	while (tmp)
	{
		printf("%c\t%p\n", tmp->data, (void *)tmp);
		tmp = tmp->next;
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
