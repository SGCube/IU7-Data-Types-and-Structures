#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "btree.h"

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

ARR_DLL void ARR_DECL free_all(tree_t *tree)
{
	if (!tree)
		return;
	tree_t *cur = tree;
	tree_t *left = cur->left, *right = cur->right;
	free_all(left);
	free_all(right);
	free(tree);
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

ARR_DLL tree_t* ARR_DECL read(FILE *f, int *rc)
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
			free_all(tree);
			return NULL;
		}
		else
		{
			*rc = OK;
			tree_t *node = create_node(x);
			if (!node)
			{
				*rc = ERR_MEMORY;
				free_all(tree);
				return NULL;
			}
			else
				tree = add(tree, node);
		}
	}
	while (*rc == OK && !feof(f));
	return tree;
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

void to_dot(tree_t *tree, FILE *f)
{
	if (tree)
	{
		to_dot(tree->left, f);
		if (tree->left)
			fprintf(f, "%d -> %d;\n", tree->data, tree->left->data);

		if (tree->right)
			fprintf(f, "%d -> %d;\n", tree->data, tree->right->data);
		to_dot(tree->right, f);
	}
}

ARR_DLL void ARR_DECL export_to_dot(FILE *f, const char *name, tree_t *tree)
{
    fprintf(f, "digraph %s {\n", name);
    to_dot(tree, f);
    fprintf(f, "}\n");
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
