#ifndef __LIST__H__
#define __LIST__H__

typedef struct node node_t

struct node
{
	int no;
	int w;
	node_t *next;
}

node_t *create_node(int no, int w);

node_t *push_back(node_t *head, node_t *node);

void free_list(node_t **head);

#endif