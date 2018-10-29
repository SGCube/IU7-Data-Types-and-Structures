#ifndef __MATRIX_H__
#define __MATRIX_H__

struct node
{
	int index;
	struct node *next;
};

struct matrix
{
	int rows;
	int cols;
	int *a;
	int *ja;
	struct node ia;
};

#endif