#ifndef __STRUCT__H__

#define __STRUCT__H__

typedef struct tree_node tree_t;

struct tree_node
{
	int data;
	tree_t *left;
	tree_t *right;
};

#endif