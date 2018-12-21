#include <stdlib.h>
#include "list.h"

node_t *create_node(int no, int w)
{
	node_t *node = malloc(sizeof(node_t));
	if (!node)
		return NULL;
	node->no = no;
	node->w = w;
	node->next = NULL;
	return node;
}

node_t *push_back(node_t *head, node_t *node)
{
	if (!node)
		return head;
	if (!head)
		return node;
	node_t *cur = head;
	if (head->no == node->no)
		return head;
	while (cur->next)
	{
		if (cur->next->no == node->no)
			return head;
		cur = cur->next;
	}
	cur->next = node;
	return head;
}

void free_list(node_t **head)
{
	node_t *cur = *head;
	while (*head)
	{
		cur = *head;
		*head = (*head)->next;
		free(cur);
	}
	*head = NULL;
}

void free_all(node_t **adj, int n)
{
	for (int i = 0; i < n; i++)
		free_list(&adj[i]);
	free(adj);
}