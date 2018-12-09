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
		for (int d = 0; d < depth; d++)
			printf("\t");
		print_node(root);
		print(root->left, depth + 1);
		print(root->right, depth + 1);
	}
}

/*ARR_DLL int ARR_DECL del_node(tree_t **root, char *data)
{
	if (!root)
		return node;
	int cmp = strcmp(node->name, tree->name);
	if (cmp == 0)
		return NULL;
	else if (cmp < 0)
		root->left = insert(root->left, node);
	else
		root->right = insert(root->right, node);
	return root;
}*/
