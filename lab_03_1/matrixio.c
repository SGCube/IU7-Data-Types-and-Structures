#include <stdio.h>
#include <stdlib.h>
#include "matrixio.h"
#include "error.h"

#define ALLOC_SIZE 20

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