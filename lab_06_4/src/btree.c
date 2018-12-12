#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "btree.h"
#include "queue.h"

ARR_DLL tree_t* ARR_DECL create_node(int data)
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
	free(node);
}

ARR_DLL tree_t* add(tree_t *root, tree_t *node)
{
	if (!root)
		return node;
	if (node->data == root->data)
		return NULL;
	else if (node->data < root->data)
		root->left = add(root->left, node);
	else
		root->right = add(root->right, node);
	return root;
}

ARR_DLL tree_t* ARR_DECL search(tree_t *root, int data)
{
	if (!root)
		return NULL;
	if (data < root->data)
		return search(root->left, data);
	if (data > root->data)
		return search(root->right, data);
	return root;
}

ARR_DLL void ARR_DECL print_node(tree_t *node)
{
	printf("%d\n", node->data);
}

void print_tree(tree_t *root)
{
	if (!root)
		return;
	
	queue_t *pin = NULL, *pout = NULL;
	int cur_level = 1, next_level = 0;
	push((void *)root, &pin, &pout);
	while (pout)
	{
		tree_t *node = pop(&pin, &pout);
		cur_level--;
		if (node)
		{
			printf("%d ", node->data);
			push((void *)node->left, &pin, &pout);
			push((void *)node->right, &pin, &pout);
			next_level += 2;
		}
		if (cur_level == 0)
		{
			printf("\n");
			cur_level = next_level;
			next_level = 0;
		}
	}
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

ARR_DLL tree_t* ARR_DECL tree_remove(tree_t *tree, int data)
{
	if (!tree)
		return NULL;
	//поиск вершины
	if (data < tree->data)
		tree->left = tree_remove(tree->left, data);
	else if (data > tree->data)
		tree->right = tree_remove(tree->right, data);
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
		//изъятие максимального элемента в левом поддереве
		tree_t *tmp = tmpl;
		while (tmp->right)
			tmp = tmp->right;
		tmpl->right = tmp->left;
		//установление новой вершины
		tmax->left = tmpl;
		tmax->right = tmpr;
		return tmax;
	}
	return tree;
};

/*ARR_DLL tree_t* ARR_DECL remove(tree_t *root, int data)
{
	if (!*root)
		return NULL;
	
	tree_t *par = NULL, *node = *root;
	int br = 0;
	while (node)
	{
		if (data < node->data)					//поиск в левом поддереве
		{
			br = -1;
			par = node;
			node = node->left;
		}
		else if (data > node->data)				//поиск в правом поддереве
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
			tree_t *tmp = remove(&lmaxpar, lmax->data);
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
}*/