#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "btree.h"
#include "avltree.h"

tree_t *rotate_left(tree_t *node)
{
	tree_t *tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	return tmp;
}

tree_t *rotate_right(tree_t *node)
{
	tree_t *tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;
	return tmp;
}

ARR_DLL int ARR_DECL height(tree_t *root)
{
	if (!root)
		return 0;
	int hleft = 0, hright = 0;
	if (root && root->left)
		hleft = height(root->left);
	if (root && root->right)
		hright = height(root->right);
	return (hleft < hright) ? hright + 1 : hleft + 1;
}

ARR_DLL tree_t* ARR_DECL balance(tree_t *tree)
{
	if (!tree)
		return NULL;
	if (tree->left)
		tree->left = balance(tree->left);
	if (tree->right)
		tree->right = balance(tree->right);
	int bfactor = height(tree->right) - height(tree->left);
	printf("%s -> bfactor: %d\t", tree->data, bfactor);
	printf("left: %d\tright: %d\n", height(tree->left), height(tree->right));
	if (bfactor < -1)
	{
		bfactor = height(tree->left->right) - height(tree->left->left);
		if (bfactor > 1)
			tree->left = rotate_left(tree->left);
		return rotate_right(tree);
	}
	else if (bfactor > 1)
	{
		bfactor = height(tree->right->right) - height(tree->right->left);
		if (bfactor < -1)
			tree->right = rotate_right(tree->right);
		return rotate_left(tree);
	}
	return tree;
}