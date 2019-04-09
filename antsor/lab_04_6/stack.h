#ifndef __STACK_H__
#define __STACK_H__

typedef struct list_t list;

struct list_t
{
	char data;
	list *next;
};

typedef struct frspc_t frspc;

struct frspc_t
{
	void *data;
	frspc *next;
};

list *create_node(char ch, frspc **space);
list *create_node_t(char ch);
void push(list **peak, int *n, list *node);
list *pop(list **peak, int *n);
void print_list(list *peak, int *n, frspc *space);
int is_palindrom(list **peak, int *n);
void free_spaces(frspc **space);
void stack_free(list **peak, int *n);

#endif