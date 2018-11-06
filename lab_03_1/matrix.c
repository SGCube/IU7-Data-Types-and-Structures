#include <stdlib.h>
#include "matrix.h"
#include "error.h"

matrix sum(matrix a, matrix b)
{
	matrix c = { a.nr, a.nc, 0, NULL, NULL, NULL };
	
	c.a = calloc(a.nk + b.nk, sizeof(int));
	c.ja = calloc(a.nk + b.nk, sizeof(int));
	c.ia = calloc(a.nr, sizeof(int));
	
	int acur = 0, bcur = 0;
	int ar = 0, br = 0;
	while (a.ia[ar] != 0)
		ar++;
	while (b.ia[br] != 0)
		br++;
	
	c.ia[0] = -1;
	while (acur < a.nk && bcur < b.nk)
	{
		if (ar == br && a.ja[acur] == b.ja[bcur])
		{
			c.a[c.nk] = a.a[acur] + b.a[bcur];
			c.ja[c.nk] = a.ja[acur];
			if (c.ia[ar] == -1)
				c.ia[ar] = c.nk
			c.nk++;
			acur++;
			bcur++;
		}
		else if (ar <= br && a.ja[acur] <= b.ja[bcur])
		{
			c.a[c.nk] = a.a[acur];
			c.ja[c.nk] = a.ja[acur];
			if (c.ia[ar] == -1)
				c.ia[ar] = c.nk
			c.nk++;
			acur++;
		}
		else if (br <= ar && b.ja[bqur] <= a.ja[acur])
		{
			c.a[c.nk] = b.a[bcur];
			c.ja[c.nk] = b.ja[bcur];
			if (c.ia[br] == -1)
				c.ia[br] = c.nk
			c.nk++;
			bcur++;
		}
	}
	
	return c;
}