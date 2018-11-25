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

int peek(list *peak);
int push(list **peak, char ch, frspc **space);
int pop(list **peak, frspc **space);
int push_new(list **peak, char ch);
void pop_free(list **peak);
void print_list(list *peak, frspc *space);
int is_palindrom(list *peak);
void free_spaces(frspc **space);

#endif