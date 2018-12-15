#ifndef __STRUCT__H__

#define __STRUCT__H__

#define MAX_SIZE 1000
#define MAX_SEARCH 4

typedef struct tree_node tree_t;

struct tree_node
{
	int data;
	tree_t *left;
	tree_t *right;
};

typedef struct hash
{
	int value;
	signed char flag;
} hash_t;

#endif