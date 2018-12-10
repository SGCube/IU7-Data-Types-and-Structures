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
	int hleft = 0, hright = 0;
	if (root->left)
		hleft += height(root->left);
	if (root->right)
		hright += height(root->right);
	return (hleft < hright) ? hright + 1 : hleft + 1;
}

ARR_DLL tree_t* ARR_DECL balance(tree_t *tree)
{
	if (tree->left)
		tree->left = balance(tree->left);
	if (tree->right)
		tree->right = balance(tree->right);
	int bfactor = height(tree->left) - height(tree->right);
	if (bfactor < -1)
	{
		bfactor = height(tree->left->left) - height(tree->left->right);
		if (bfactor > 1)
			tree->left = rotate_left(tree->left);
		return rotate_right(tree);
	}
	else if (bfactor > 1)
	{
		bfactor = height(tree->right->left) - height(tree->right->right);
		if (bfactor < -1)
			tree->right = rotate_right(tree->right);
		return rotate_left(tree);
	}
	return tree;
}