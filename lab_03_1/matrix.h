#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct
{
	int nr;		//number of rows
	int nc;		//number of columns
	int nk;		//number of non-zero values
	int *a;		//non-zero values
	int *ja;	//column index of a values
	int *ia;	//indexes of first elements in rows
} matrix;

int matrix_init(matrix *ma, int nr, int nc, int nk);
int matrix_random(matrix *ma);
void sum(matrix a, matrix b, matrix *c);

#endif