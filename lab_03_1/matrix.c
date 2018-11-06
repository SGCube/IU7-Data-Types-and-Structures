#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "error.h"

void sum(matrix a, matrix b, matrix *c)
{
	int acur = 0, bcur = 0;
	int ar = 0, br = 0;
	int x = 0;
	while (a.ia[ar] != 0)
		ar++;
	while (b.ia[br] != 0)
		br++;
	
	for (int i = 0; i < c->nr; i++)
		c->ia[i] = -1;
	
	while (acur < a.nk && bcur < b.nk)
	{
		if (ar == br && a.ja[acur] == b.ja[bcur])
		{
			x = a.a[acur] + b.a[bcur];
			if (x != 0)
			{
				c->a[c->nk] = x;
				c->ja[c->nk] = a.ja[acur];
				if (c->ia[ar] == -1)
					c->ia[ar] = c->nk;
				c->nk += 1;
			}
			acur++;
			bcur++;
		}
		else if (ar < br || (ar == br && a.ja[acur] <= b.ja[bcur]))
		{
			if (a.a[acur] != 0)
			{
				c->a[c->nk] = a.a[acur];
				c->ja[c->nk] = a.ja[acur];
				if (c->ia[ar] == -1)
					c->ia[ar] = c->nk;
				c->nk += 1;
			}
			acur++;
		}
		else if (br < ar || (ar == br && b.ja[bcur] <= a.ja[acur]))
		{
			if (b.a[bcur] != 0)
			{
				c->a[c->nk] = b.a[bcur];
				c->ja[c->nk] = b.ja[bcur];
				if (c->ia[br] == -1)
					c->ia[br] = c->nk;
				c->nk += 1;
			}
			bcur++;
		}
		if (ar < a.nr - 1 && acur >= a.ia[ar + 1])
		{
			do
				ar++;
			while (a.ia[ar] == acur);
			ar--;
		}
		if (br < b.nr - 1 && bcur >= b.ia[br + 1])
		{
			do
				br++;
			while (b.ia[br] == bcur);
			br--;
		}
	}
	while (acur < a.nk)
	{
		c->a[c->nk] = a.a[acur];
		c->ja[c->nk] = a.ja[acur];
		if (c->ia[ar] == -1)
			c->ia[ar] = c->nk;
		acur++;
		c->nk += 1;
	}
	while (bcur < b.nk)
	{
		c->a[c->nk] = b.a[bcur];
		c->ja[c->nk] = b.ja[bcur];
		if (c->ia[br] == -1)
			c->ia[br] = c->nk;
		bcur++;
		c->nk += 1;
	}
}