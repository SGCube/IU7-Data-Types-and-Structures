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
	if (f == stdout && ma.nk == 0)
		fprintf(f, "No non-zero values!\n");
	if (f != stdout)
		fprintf(f, "%d %d %d\n", ma.nr, ma.nc, ma.nk);
	
	if (ma.nk == 0)
		return;
	
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
	int cj = 0, ck = 0;
	if (f == stdout && ma.nk == 0)
		fprintf(f, "No non-zero values!\n");
	if (f != stdout)
		fprintf(f, "%d %d\n", ma.nr, ma.nc);
	for (int i = 0; i < ma.nr; i++)
	{
		for (; ck < ma.ia[i]; ck++)
		{
			for (; cj < ma.ja[ck] * (i + 1); cj++)
				fprintf(f, "0 ");
			if (cj % ma.nc != 0)
				fprintf(f, "%d ", ma.a[ck]);
		}
		fprintf(f, "\n");
	}
}

int read_matrix(matrix *ma, FILE *f)
{
	int x, rc;
	if (f == stdin)
		printf("Enter size of matrix: ");
	rc = fscanf(f, "%d %d", &ma->nr, &ma->nc);
	if (rc == EOF)
		return ERR_EMPTY;
	if (rc != 2)
		return ERR_SIZE;
	if (ma->nr < 1 || ma->nc < 1)
		return ERR_SIZE;
	
	ma->a = calloc(ALLOC_SIZE, sizeof(int));
	ma->ja = calloc(ALLOC_SIZE, sizeof(int));
	ma->ia = calloc(ma->nr, sizeof(int));
	
	if (!ma->a || !ma->ja || !ma->ia)
		return ERR_ALLOC;
	
	if (f == stdin)
		printf("Enter matrix:\n");
	for (int i = 0; i < ma->nr; i++)
	{
		ma->ia[i] = -1;
		for (int j = 0; j < ma->nc; j++)
		{
			rc = fscanf(f, "%d", &x);
			if (rc == EOF)
				return ERR_MISSED_DATA;
			if (rc != 1)
				return ERR_MATRIX;
			if (x != 0)
			{
				if (ma->nk % ALLOC_SIZE == 0)
				{
					void *ta = realloc(ma->a,
						(ma->nk + ALLOC_SIZE) * sizeof(int));
					if (!ta)
						return ERR_ALLOC;
					ma->a = (int *) ta;
					
					void *tj = realloc(ma->ja,
						(ma->nk + ALLOC_SIZE) * sizeof(int));
					if (!tj)
						return ERR_ALLOC;
					ma->ja = (int *) tj;
				}
				ma->a[ma->nk] = x;
				ma->ja[ma->nk] = j;
				if (ma->ia[i] == -1)
					ma->ia[i] = ma->nk;
				ma->nk += 1;
			}
		}
		if (ma->ia[i] == -1)
			ma->ia[i] = ma->nk;
	}
	
	return OK;
}