#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct list_t list;

struct list_t
{
	int data;
	list *next;
};

int push_queue(int val, list **pin, list **pout);

int pop_queue(list **pin, list **pout);

void print_queue(list **pin, list **pout);

#endif