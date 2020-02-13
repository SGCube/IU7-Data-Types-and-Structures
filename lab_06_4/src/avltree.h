#ifndef __AVLTREE__H__

#define __AVLTREE__H__

int height(tree_t *root);

tree_t* balance(tree_t *tree);

tree_t* tree_balance(tree_t *tree);

tree_t* avl_remove(tree_t *tree, int data, int *kcmp);

#endif