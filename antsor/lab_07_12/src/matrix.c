#include <stdlib.h>
#include "matrix.h"

int *alloc_matrix(int n, int m)
{
	if (n <= 0 || m <= 0)
		return NULL;
	
	int *ma = malloc(n * m * sizeof(int));
	if (!ma)
		return NULL;
	
	return ma;
}

void free_matrix(int *ma)
{
	if (ma)
		free(ma);
}
