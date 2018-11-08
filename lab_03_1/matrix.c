#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "error.h"

#define RAND_N 100

void debug_print(matrix ma)
{
	printf("A\t");
	for (int i = 0; i < ma.nk; i++)
		printf("%d ", ma.a[i]);
	printf("\n");
	
	printf("JA\t");
	for (int i = 0; i < ma.nk; i++)
		printf("%d ", ma.ja[i]);
	printf("\n");
	
	printf("IA\t");
	for (int i = 0; i < ma.nr; i++)
		printf("%d ", ma.ia[i]);
	printf("\n");
}

int compare(const void *x1, const void *x2)
{
	return *(int*)x1 - *(int*)x2;
}

int matrix_init(matrix *ma, int nr, int nc, int nk)
{
	ma->nr = nr;
	ma->nc = nc;
	ma->nk = 0;
	ma->a = calloc(nk, sizeof(int));
	ma->ja = calloc(nk, sizeof(int));
	ma->ia = calloc(nr, sizeof(int));
	if (!ma->a || !ma->ja || !ma->ia)
		return ERR_ALLOC;
	return OK;
}

int matrix_invert(matrix *ma, matrix mb)
{
	ma->nr = mb.nr;
	ma->nc = mb.nc;
	ma->nk = mb.nk;
	ma->a = calloc(mb.nk, sizeof(int));
	ma->ja = calloc(mb.nk, sizeof(int));
	ma->ia = calloc(mb.nr, sizeof(int));
	if (!ma->a || !ma->ja || !ma->ia)
		return ERR_ALLOC;
	for (int k = 0; k < ma->nk; k++)
	{
		ma->a[k] = -mb.a[k];
		ma->ja[k] = mb.ja[k];
	}
	for (int i = 0; i < ma->nr; i++)
		ma->ia[i] = mb.ia[i];
	return OK;
}

void matrix_random(matrix *ma)
{
	for (int i = 0; i < ma->nr; i++)
		ma->ia[i] = -1;
	
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
}

int matrix_random_set(matrix *ma)
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
	
	if (ma->nk == 0)
		return OK;
	matrix_random(ma);
	return OK;
}

int *matrix_std_null(int ro, int co)
{
	if (ro < 1 || co < 1)
		return NULL;
	int *mstd = calloc(ro * co, sizeof(int *));
	if (!mstd)
		return NULL;
	return mstd;
}

int *matrix_std(matrix ma)
{
	int *mstd = matrix_std_null(ma.nr, ma.nc);
	if (!mstd)
		return NULL;
	
	for (int i = 0; i < ma.nr; i++)
	{
		int ilast = ma.nk;
		if (i < ma.nr - 1)
			ilast = ma.ia[i + 1];
		for (int ck = 0; ck < ilast; ck++)
			mstd[i * ma.nc + ma.ja[ck]] = ma.a[ck];
	}
	return mstd;
}

void sum(matrix a, matrix b, matrix *c)
{
	int acur = 0, bcur = 0;
	int ar = 0, br = 0;
	int x = 0;
	do
		ar++;
	while (ar < a.nr && a.ia[ar] == acur);
	ar--;
	do
		br++;
	while (br < b.nr && b.ia[br] == bcur);
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
			while (ar < a.nr && a.ia[ar] == acur);
			ar--;
		}
		if (br < b.nr - 1 && bcur >= b.ia[br + 1])
		{
			do
				br++;
			while (br < b.nr && b.ia[br] == bcur);
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
		if (ar < a.nr - 1 && acur >= a.ia[ar + 1])
		{
			do
				ar++;
			while (ar < a.nr && a.ia[ar] == acur);
			ar--;
		}
	}

	while (bcur < b.nk)
	{
		c->a[c->nk] = b.a[bcur];
		c->ja[c->nk] = b.ja[bcur];
		if (c->ia[br] == -1)
			c->ia[br] = c->nk;
		bcur++;
		c->nk += 1;
		if (br < b.nr - 1 && bcur >= b.ia[br + 1])
		{
			do
				br++;
			while (br < b.nr && b.ia[br] == bcur);
			br--;
		}
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

void sum_std(int *a, int *b, int *c, int ro, int co)
{
	for (int i = 0; i < ro; i++)
		for (int j = 0; j < co; j++)
			c[i * co + j] = a[i * co + j] + b[i * co + j];
}