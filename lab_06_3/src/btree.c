#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "btree.h"

ARR_DLL tree_t* ARR_DECL create_node(char *data)
{
	tree_t *node = malloc(sizeof(tree_t));
	if (!node)
		return NULL;
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return node;
}

ARR_DLL void ARR_DECL free_node(tree_t *node)
{
	free(node->data);
	free(node);
}

ARR_DLL tree_t* add(tree_t *root, tree_t *node)
{
	if (!root)
		return node;
	int cmp = strcmp(node->data, root->data);
	if (cmp == 0)
		return NULL;
	else if (cmp < 0)
		root->left = add(root->left, node);
	else
		root->right = add(root->right, node);
	return root;
}

ARR_DLL tree_t* ARR_DECL search(tree_t *root, char *data)
{
	if (!root)
		return NULL;
	int cmp = strcmp(data, root->data);
	if (cmp < 0)
		return search(root->left, data);
	if (cmp > 0)
		return search(root->right, data);
	return root;
}

ARR_DLL void ARR_DECL print_node(tree_t *node)
{
	printf("%s\n", node->data);
}

ARR_DLL void ARR_DECL print(tree_t *root, int depth)
{
	if (root)
	{
		if (depth != 0)
			printf("\t");
		print_node(root);
		
		if (root->left)
		{
			for (int d = 0; d < depth; d++)
				printf("\t");
			printf("L: ");
			print(root->left, depth + 1);
		}
		if (root->right)
		{
			for (int d = 0; d < depth; d++)
				printf("\t");
			printf("R: ");
			print(root->right, depth + 1);
		}
	}
}

ARR_DLL tree_t* ARR_DECL del_node(tree_t **root, char *data)
{
	if (!*root)
		return NULL;
	
	tree_t *par = NULL, *node = *root;
	int br = 0;
	while (node)
	{
		int cmp = strcmp(node->data, data);
		if (cmp < 0)		//поиск в левом поддереве
		{
			br = -1;
			par = node;
			node = node->left;
		}
		else if (cmp > 0)	//поиск в правом поддереве
		{
			br = 1;
			par = node;
			node = node->right;
		}
		else if (node->left && node->right)		//два сына
		{
			//поиск максимального элемента в левом поддереве
			tree_t *lmax = node->left, *lmaxpar = node;
			while (lmax->right)
			{
				lmaxpar = lmax;
				lmax = lmax->right;
			}
			//изъятие элемента и установление его новых связей
			tree_t *tmp = del_node(&lmaxpar, lmax->data);
			tmp->left = node->left;
			tmp->right = node->right;
			//новые связи родителя удаляемого узла
			if (br < 0)
				par->left = tmp;
			else if (br > 0)
				par->right = tmp;
			else
				*root = tmp;
			return node;
		}
		else if (node->left)					//один сын (слева)
		{
			//новые связи родителя удаляемого узла
			if (br < 0)
				par->left = node->left;
			else if (br > 0)
				par->right = node->left;
			else
				*root = node->left;
			return node;
		}
		else if (node->right)					//один сын (справа)
		{
			//новые связи родителя удаляемого узла
			if (br < 0)
				par->left = node->right;
			else if (br > 0)
				par->right = node->right;
			else
				*root = node->right;
			return node;
		}
		else									//терминал
		{
			//новые связи родителя удаляемого узла
			if (br < 0)
				par->left = NULL;
			else if (br > 0)
				par->right = NULL;
			else
				*root = NULL;
			return node;
		}
	}
	return NULL;		//элемент не найден
}
