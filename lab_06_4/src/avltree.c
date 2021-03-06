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

int height(tree_t *root)
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

tree_t *balance(tree_t *tree)
{
	if (!tree)
		return NULL;
	int bfactor = height(tree->right) - height(tree->left);
	if (bfactor < -1)
	{
		bfactor = height(tree->left->right) - height(tree->left->left);
		if (bfactor > 0)
			tree->left = rotate_left(tree->left);
		return rotate_right(tree);
		
	}
	else if (bfactor > 1)
	{
		bfactor = height(tree->right->right) - height(tree->right->left);
		if (bfactor < 0)
			tree->right = rotate_right(tree->right);
		return rotate_left(tree);
	}
	return tree;
}

tree_t *tree_balance(tree_t *tree)
{
	if (!tree)
		return NULL;
	
	if (tree->left)
		tree->left = tree_balance(tree->left);
	
	if (tree->right)
		tree->right = tree_balance(tree->right);
	
	while (abs(height(tree->right) - height(tree->left)) > 1)
		tree = balance(tree);
	
	if (tree->left)
		tree->left = tree_balance(tree->left);
		
	if (tree->right)
		tree->right = tree_balance(tree->right);
	
	return tree;
}

tree_t *avl_remove(tree_t *tree, int data, int *kcmp)
{
	if (!tree)
		return NULL;
	//поиск вершины
	*kcmp += 1;
	if (data < tree->data)
		tree->left = avl_remove(tree->left, data, kcmp);
	else if (data > tree->data)
		tree->right = avl_remove(tree->right, data, kcmp);
	else //вершина найдена
	{
		//сохранение поддеревьев
		tree_t *tmpl = tree->left;
		tree_t *tmpr = tree->right;
		free(tree);
		//перестановка ветви при отсутствии левой
		if (!tmpl)
			return tmpr;
		//поиск максимального элемента в левом поддереве
		tree_t *tmax = tmpl;
		while (tmax->right)
			tmax = tmax->right;
		if (tmax == tmpl)
		{
			tmax->right = tmpr;
			return balance(tmax);
		}
		//изъятие максимального элемента в левом поддереве
		tree_t *tmp = tmpl;
		while (tmp->right)
			tmp = tmp->right;
		tmpl->right = tmp->left;
		//установление новой вершины
		tmax->left = balance(tmpl);
		tmax->right = tmpr;
		return balance(tmax);
	}
	return balance(tree);
}
