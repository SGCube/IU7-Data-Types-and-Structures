#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "btree.h"

tree_t *create_node(int data)
{
	tree_t *node = malloc(sizeof(tree_t));
	if (!node)
		return NULL;
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void tree_free(tree_t *tree)
{
	if (!tree)
		return;
	tree_t *cur = tree;
	tree_t *left = cur->left, *right = cur->right;
	tree_free(left);
	tree_free(right);
	free(tree);
}

tree_t* tree_add(tree_t *root, tree_t *node)
{
	if (!root)
		return node;
	if (node->data == root->data)
		return NULL;
	else if (node->data < root->data)
		root->left = tree_add(root->left, node);
	else
		root->right = tree_add(root->right, node);
	return root;
}

tree_t *tree_read(FILE *f, int *rc)
{
	int x;
	*rc = ERR_EMPTY;
	tree_t *tree = NULL;
	do
	{
		int sc = fscanf(f, "%d", &x);
		if (sc == EOF && *rc != ERR_EMPTY)
		{
			*rc = OK;
			return tree;
		}
		if (sc != 1 && *rc != ERR_EMPTY)
		{
			*rc = ERR_NUMB;
			tree_free(tree);
			return NULL;
		}
		else
		{
			*rc = OK;
			tree_t *node = create_node(x);
			if (!node)
			{
				*rc = ERR_MEMORY;
				tree_free(tree);
				return NULL;
			}
			else
				tree = tree_add(tree, node);
		}
	}
	while (*rc == OK && !feof(f));
	return tree;
}

tree_t *tree_search(tree_t *root, int data, int *kcmp)
{
	if (!root)
		return NULL;
	*kcmp += 1;
	if (data < root->data)
		return tree_search(root->left, data, kcmp);
	if (data > root->data)
		return tree_search(root->right, data, kcmp);
	return root;
}

void tree_print(tree_t *root, int depth)
{
	if (root)
	{
		if (depth != 0)
			printf("\t");
		printf("%d\n", root->data);
		
		if (root->left)
		{
			for (int d = 0; d < depth; d++)
				printf("\t");
			printf("L: ");
			tree_print(root->left, depth + 1);
		}
		if (root->right)
		{
			for (int d = 0; d < depth; d++)
				printf("\t");
			printf("R: ");
			tree_print(root->right, depth + 1);
		}
	}
}

void to_dot(tree_t *tree, FILE *f)
{
	if (tree)
	{
		to_dot(tree->left, f);
		if (tree->left)
			fprintf(f, "%d -> %d [color=blue];\n",
				tree->data, tree->left->data);

		if (tree->right)
			fprintf(f, "%d -> %d [color=green];\n",
				tree->data, tree->right->data);
		to_dot(tree->right, f);
	}
}

void export_to_dot(FILE *f, const char *name, tree_t *tree)
{
    fprintf(f, "digraph %s {\n", name);
    to_dot(tree, f);
    fprintf(f, "}\n");
}

tree_t *tree_remove(tree_t *tree, int data, int *kcmp)
{
	if (!tree)
		return NULL;
	//поиск вершины
	*kcmp += 1;
	if (data < tree->data)
		tree->left = tree_remove(tree->left, data, kcmp);
	else if (data > tree->data)
		tree->right = tree_remove(tree->right, data, kcmp);
	else //вершина найдена
	{
		//сохранение поддеревьев
		tree_t *tmpl = tree->left;
		tree_t *tmpr = tree->right;
		free(tree);
		//перестановка ветви при отсутствии левой
		if (!tmpl && !tmpr)
			return NULL;
		if (!tmpl)
			return tmpr;
		if (!tmpr)
			return tmpl;
		//поиск максимального элемента в левом поддереве
		tree_t *tmax = tmpl;
		while (tmax->right)
			tmax = tmax->right;
		if (tmax == tmpl)
		{
			tmax->right = tmpr;
			return tmax;
		}
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
}
