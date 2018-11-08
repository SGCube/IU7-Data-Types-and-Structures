#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "error.h"

#define RAND_N 100

int compare(const void *x1, const void *x2)
{
	return *(int*)x1 - *(int*)x2;
}

int matrix_init(matrix *ma, int nr, int nc, int nk)
{
	ma->nr = nr;
	ma->nc = nc;
	ma->nk = 0;
	ma->a = calloc(ma->nk + ma->nk, sizeof(int));
	ma->ja = calloc(ma->nk + ma->nk, sizeof(int));
	ma->ia = calloc(ma->nr, sizeof(int));
	if (!ma->a || !ma->ja || !ma->ia)
		return ERR_ALLOC;
	return OK;
}

int matrix_random(matrix *ma)
{
	printf("Generating large random matrix\n");
	printf("Enter number of non-null elements: ");
	if (scanf("%d", &ma->nk) != 1 || ma->nk < 0 || ma->nk > ma->nr * ma->nc)
		return ERR_AMOUNT;
	
	if (ma->nk == 0)
	{
		ma->a = calloc(1, sizeof(int));
		ma->ja = calloc(1, sizeof(int));
	}
	else
	{
		ma->a = calloc(ma->nk, sizeof(int));
		ma->ja = calloc(ma->nk, sizeof(int));
	}
	
	ma->ia = calloc(ma->nr, sizeof(int));
	if (!ma->a || !ma->ja || !ma->ia)
		return ERR_ALLOC;
	for (int i = 0; i < ma->nr; i++)
		ma->ia[i] = -1;
	
	if (ma->nk == 0)
		return OK;
	
	int i, j, ii;
	short int not_ok = 0;
	for (int k = 0; k < ma->nk; k++)
	{
		do
		{
			not_ok = 0;
			j = rand() % (ma-> nr * ma-> nc);
			for(int kk = k - 1; kk > 0 && !not_ok; kk--)
				if (ma->ja[kk] == j)
					not_ok = 1;
		}
		while(not_ok);
		ma->ja[k] = j;
	}
	qsort(ma->ja, ma->nk, sizeof(int), compare);
	for (int k = 0; k < ma->nk; k++)
	{
		i = ma->ja[k] / ma->nr;
		j = ma->ja[k] % ma->nc;
		do
			ma->a[k] = rand() % (2 * RAND_N) - RAND_N;
		while(ma->a[k] == 0);
		ma->ja[k] = j;
		
		if (ma->ia[i] == -1)
			ma->ia[i] = k;
	}
	ii = ma->nk;
	for (int i = ma->nr - 1; i >= 0; i--)
	{
		if (ma->ia[i] == -1)
			ma->ia[i] = ii;
		else
			ii = ma->ia[i];
	}
	return OK;
}

void sum(matrix a, matrix b, matrix *c)
{
	int acur = 0, bcur = 0;
	int ar = 0, br = 0;
	int x = 0;
	do
		ar++;
	while (a.ia[ar] == acur && ar < a.nr);
	ar--;
	do
		br++;
	while (b.ia[br] == bcur && br < b.nr);
	br--;
	
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
	int ii = c->nk;
	for (int i = c->nr - 1; i >= 0; i--)
	{
		if (c->ia[i] == -1)
			c->ia[i] = ii;
		else
			ii = c->ia[i];
	}
}