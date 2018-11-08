#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "matrixio.h"
#include "fileio.h"
#include "error.h"

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	srand(time(NULL));
	int rc = OK;
	matrix a = { 0, 0, 0, NULL, NULL, NULL };
	matrix b = { 0, 0, 0, NULL, NULL, NULL };
	matrix c = { 0, 0, 0, NULL, NULL, NULL };
	
	FILE *f1 = NULL, *f2 = NULL, *fout = NULL;
	FILE *fa = stdin, *fb = stdin, *fc = stdout;
	
	if (file_open(argc, argv, &f1, &f2, &fout) == ERR_FILE)
	{
		printf("File open error!\n");
		return ERR_FILE;
	}
	
	if (argc >= 2)
		fa = f1;
	if (argc >= 3)
		fb = f2;
	if (argc >= 4)
		fc = fout;
	
	rc = read_matrix(&a, fa);
	if (rc != OK)
	{
		errmsg(rc);
		free_matrix(a);
		file_close(fa, fb, fc);
		return rc;
	}
	printf("Matrix A: \n");
	print_matrix(a, stdout);
	printf("\n");
	if (a.nc * a.nr < 2 * LARGE_SIZE)
	{
		print_matrix_std(a, stdout);
		printf("\n");
	}
	
	rc = read_matrix(&b, fb);
	if (rc != OK)
	{
		errmsg(rc);
		free_matrix(a);
		free_matrix(b);
		file_close(fa, fb, fc);
		return rc;
	}
	printf("Matrix B: \n");
	print_matrix(b, stdout);
	printf("\n");
	if (b.nc * b.nr < 2 * LARGE_SIZE)
	{
		print_matrix_std(b, stdout);
		printf("\n");
	}
	
	c.nr = a.nr;
	c.nc = a.nc;
	c.nk = 0;
	c.a = calloc(a.nk + b.nk, sizeof(int));
	c.ja = calloc(a.nk + b.nk, sizeof(int));
	c.ia = calloc(a.nr, sizeof(int));
	if (!c.a || !c.ja || !c.ia)
	{
		printf("Memory allocation error!\n");
		free_matrix(a);
		free_matrix(b);
		free_matrix(c);
		file_close(fa, fb, fc);
		return ERR_ALLOC;
	}
	
	sum(a, b, &c);
	printf("Matrix C: \n");
	print_matrix(c, stdout);
	printf("\n");
	if (c.nc * c.nr < 2 * LARGE_SIZE)
	{
		print_matrix_std(c, stdout);
		printf("\n");
	}
	
	free_matrix(a);
	free_matrix(b);
	free_matrix(c);
	file_close(fa, fb, fc);
	return rc;
}