#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct queue queue_t;

struct queue
{
	void *data;
	queue_t *next;
};

int push(void *data, queue_t **pin, queue_t **pout);

void *pop(queue_t **pin, queue_t **pout);

#endif