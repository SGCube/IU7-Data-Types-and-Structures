#include <stdlib.h>
#include "matrix.h"
#include "error.h"

matrix sum(matrix a, matrix b)
{
	matrix c = { a.nr, a.nc, 0, NULL, NULL, NULL};
	
	c.a = calloc(a.nk + b.nk, sizeof(int));
	c.ja = calloc(a.nk + b.nk, sizeof(int));
	c.ia = calloc(a.nr, sizeof(int));
	
	int acur = 0;
	int bcur = 0;
	for (int i = 0; i < c.nr; i++)
	{
		int ilast = ma.nk;
		int cj = 0;
		if (i < ma.nr - 1)
			ilast = ma.ia[i + 1];
		for (; ck < ilast; ck++)
		{
			for (; cj < ma.ja[ck]; cj++)
				fprintf(f, "0 ");
			fprintf(f, "%d ", ma.a[ck]);
			cj++;
		}
		for (; cj < ma.nc; cj++)
			fprintf(f, "0 ");
		fprintf(f, "\n");
	}
	
	return c;
}