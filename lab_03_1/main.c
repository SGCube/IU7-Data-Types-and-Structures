#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "matrixio.h"
#include "fileio.h"
#include "error.h"

int readmatr(matrix *ma, FILE *f, char ltr)
{
	if (f == NULL)
		return ERR_FILE;
	if (f == stdin)
	{
		printf("Matrix %c read:\n", ltr);
		return read_matrix_std(ma, f);
	}
	char ch;
	printf("Matrix %c set in file?\n"
		"1 - standart\n"
		"other - vectors for non-null\n", ltr);
	fflush(stdin);
	scanf("%c", &ch);
	if (ch == '1')
		return read_matrix_std(ma, f);
	return read_matrix(ma, f);
}

void printmatr(matrix ma, FILE *f)
{
	print_matrix(ma, f);
	printf("\n");
	if (ma.nc * ma.nr <= 2 * LARGE_SIZE && ma.nk > 0)
	{
		print_matrix_std(ma, f);	
		printf("\n");
	}
}

void print_matrs(int *a, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			printf("%d ", a[i * m + j]);
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	srand(time(NULL));
	
	int rc = OK;
	matrix a = { 0, 0, 0, NULL, NULL, NULL };
	matrix b = { 0, 0, 0, NULL, NULL, NULL };
	matrix c = { 0, 0, 0, NULL, NULL, NULL };
	
	FILE *f1 = stdin, *f2 = stdin, *fout = stdout;
	if (file_open(argc, argv, &f1, &f2, &fout) == ERR_FILE)
	{
		errmsg(rc);
		return ERR_FILE;
	}
	
	rc = readmatr(&a, f1, 'A');
	printf("\n");
	if (rc == OK)
	{
		rc = readmatr(&b, f2, 'B');
		printf("\n");
	}
	if (rc == OK)
	{
		if (a.nr != b.nr || a.nc != b.nc)
			rc = ERR_DIFF_SIZE;
	}
	if (rc == OK)
		rc = matrix_init(&c, a.nr, a.nc, a.nk + b.nk);
	if (rc == OK)
	{
		sum(a, b, &c);
		printf("Matrix A:\n");
		printmatr(a, stdout);
		printf("Matrix B:\n");
		printmatr(b, stdout);
		printf("Matrix C = A + B:\n");
		printmatr(c, stdout);
		
		printf("Would you like to operate standart sum? (1 - yes):\n");
		char ch;
		fflush(stdin);
		
		scanf("%c", &ch);
		if (ch == '1')
		{
			int *ma = matrix_std(a);
			int *mb = matrix_std(b);
			int *mc = matrix_std_null(a.nr, a.nc);
			if (!ma || !mb || !mc)
				rc = ERR_ALLOC;
			else
			{
				sum_std(ma, mb, mc, a.nr, a.nc);
				printf("\nMatrix A:\n");
				print_matrs(ma, a.nr, a.nc);
				printf("\nMatrix B:\n");
				print_matrs(mb, a.nr, a.nc);
				printf("\nMatrix C = A + B:\n");
				print_matrs(mc, a.nr, a.nc);
			}
		}
	}
	
	free_matrix(a);
	free_matrix(b);
	free_matrix(c);
	file_close(f1, f2, fout);
	errmsg(rc);
	return rc;
}