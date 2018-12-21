#ifndef __BTREE__H__

#define __BTREE__H__

#include <stdio.h>

typedef struct tree_node tree_t;

struct tree_node
{
	int data;
	tree_t *left;
	tree_t *right;
};

tree_t *create_node(int data);

void tree_free(tree_t *tree);

tree_t *tree_add(tree_t *root, tree_t *node);

tree_t *tree_read(FILE *f, int *rc);

tree_t *tree_search(tree_t *root, int data, int *kcmp);

tree_t *tree_remove(tree_t *root, int data, int *kcmp);

void export_to_dot(FILE *f, const char *name, tree_t *tree);

void tree_print(tree_t *root, int depth);

#endif