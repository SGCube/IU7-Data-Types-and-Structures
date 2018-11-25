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
int matrix_invert(matrix *ma, matrix mb);
int matrix_random(matrix *ma);
int matrix_random_set(matrix *ma);
int *matrix_std_null(int ro, int co);
int *matrix_std(matrix ma);
void sum(matrix a, matrix b, matrix *c);
void sum_std(int *a, int *b, int *c, int ro, int co);

#endif