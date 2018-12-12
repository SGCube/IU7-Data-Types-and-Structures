#include <stdlib.h>
#include "queue.h"
#include "error.h"

int push(void *data, queue_t **pin, queue_t **pout)
{
	queue_t *node = malloc(sizeof(queue_t));
	if (!node)
		return ERR_MEMORY;
	node->data = data;
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

void *pop(queue_t **pin, queue_t **pout)
{
	if (!*pout)
		return NULL;
	if (*pin == *pout)
		*pin = NULL;
	void *rc = (*pout)->data;
	queue_t *tmp = *pout;
	*pout = (*pout)->next;
	free(tmp);
	return rc;
}