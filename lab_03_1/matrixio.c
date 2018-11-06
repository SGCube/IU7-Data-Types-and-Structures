#include <stdio.h>
#include <stdlib.h>
#include "matrixio.h"
#include "error.h"

#define ALLOC_SIZE 20

void free_matrix(matrix ma)
{
	ma.nr = 0;
	ma.nc = 0;
	ma.nk = 0;
	if (ma.a)
		free(ma.a);
	if (ma.ja)
		free(ma.ja);
	if (ma.ia)
		free(ma.ia);
}

void print_matrix(matrix ma, FILE *f)
{
	if (f == stdout && nk == 0)
		fprintf(f, "No non-zero values!\n");
	if (f != stdout)
		fprintf(f, "%d %d\n", ma.nr, ma.nc, ma.nk);
	
	if (f == stdout)
		fprintf(f, "A\t");
	for (int i = 0; i < ma.nk; i++)
		fprintf(f, "%d ", ma.a[i]);
	fprintf(f, "\n");
	
	if (f == stdout)
		fprintf(f, "JA\t");
	for (int i = 0; i < ma.nk; i++)
		fprintf(f, "%d ", ma.ja[i]);
	fprintf(f, "\n");
	
	if (f == stdout)
		fprintf(f, "IA\t");
	for (int i = 0; i < ma.nr; i++)
		fprintf(f, "%d ", ma.ia[i]);
	fprintf(f, "\n");
}

void print_matrix_std(matrix ma, FILE *f)
{
	int ci = 0, cj = 0, ck = 0;
	if (f == stdout && nk == 0)
		fprintf(f, "No non-zero values!\n");
	if (f != stdout)
		fprintf(f, "%d %d\n", ma.nr, ma.nc);
	for (int i = 0; i < nr; i++)
	{
		for (; ck < ma.ia[i]; ck++)
		{
			for (; cj < ma.ja[ck] * (i + 1); cj++)
				fprintf(f, "0 ");
			if (cj % nc != 0)
				fprintf(f, "%d ", ma.a[ck]);
		}
		fprintf(f, "\n");
	}
}

int read_matrix(matrix ma, FILE *f)
{
	int x, rc;
	if (f == stdin)
		printf("Enter size of matrix: ");
	int rc = fscanf(f, "%d %d", &ma.nr, &ma.nc);
	if (rc == EOF)
		return ERR_EMPTY;
	if (rc != 2)
		return ERR_SIZE;
	if (ma.nr < 1 || ma.nc < 1)
		return ERR_SIZE;
	
	a = calloc(ALLOC_SIZE * sizeof(int));
	ja = calloc(ALLOC_SIZE * sizeof(int));
	ia = calloc(ma.nr * sizeof(int));
	
	if (!a || !ja || !ia)
		return ERR_ALLOC;
	
	if (f == stdin)
		printf("Enter matrix:\n");
	for (int i = 0; i < ma.nr; i++)
	{
		ia[i] = -1;
		for (int j = 0; j < ma.nc; j++)
		{
			rc = fscanf(f, "%d", &x);
			if (rc == EOF)
				return ERR_MISSED_DATA;
			if (rc != 1)
				return ERR_MATRIX;
			if (x != 0)
			{
				if (nk % ALLOC_SIZE == 0)
				{
					void *ta = realloc(a, (nk + ALLOC_SIZE) * sizeof(int));
					if (!ta)
						return ERR_ALLOC;
					a = (int *) ta;
					
					void *tj = realloc(ja, (nk + ALLOC_SIZE) * sizeof(int));
					if (!tj)
						return ERR_ALLOC;
					ja = (int *) tj;
				}
				a[nk] = x;
				ja[nk++] = j;
				if (ia[i] == -1)
					ia[i] = nk - 1;
			}
		}
		if (ia[i] == -1)
			ia[i] = nk;
	}
	
	return OK;
}