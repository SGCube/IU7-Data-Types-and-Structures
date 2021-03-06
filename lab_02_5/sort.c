#include <stdio.h>
#include "sort.h"

typedef enum { FALSE, TRUE } t_bool;

void bsort_key(struct keytable *keys, int len)
{
	struct keytable temp;
	t_bool sorted = TRUE;
	for (int i = 0; i < len; i++)
	{
		sorted = TRUE;
		for (int j = 1; j < len - i; j++)
			if (keys[j - 1].val > keys[j].val)
			{
				temp = keys[j];
				keys[j] = keys[j - 1];
				keys[j - 1] = temp;
				sorted = FALSE;
			}
		if (sorted)
			break;
	}
}

void qsort_key(struct keytable *pb, struct keytable *pe)
{
	struct keytable *pleft = pb, *pright = pe;
	struct keytable *pm = pb + (pe - pb) / 2;
	while (pleft < pright)
	{
		while (pright->val >= pm->val && pleft < pright)
			pright--;
		if (pleft != pright)
		{
			*pleft = *pright;
			pleft++;
		}
		while (pleft->val <= pm->val && pleft < pright)
			pleft++;
		if (pleft != pright)
		{
			*pright = *pleft;
			pright--;
		}
	}
	*pleft = *pm;
	pm = pleft;
	pleft = pb;
	pright = pe;
	if (pleft < pm)
		qsort_key(pleft, pm);
	if (pright > pm)
		qsort_key(pm + 1, pright);
}

void bsort_rep(struct spectac *rep, int len)
{
	struct spectac temp;
	t_bool sorted = TRUE;
	for (int i = 0; i < len; i++)
	{
		sorted = TRUE;
		for (int j = 1; j < len - i; j++)
			if (rep[j - 1].max_tprice > rep[j].max_tprice)
			{
				temp = rep[j];
				rep[j] = rep[j - 1];
				rep[j - 1] = temp;
				sorted = FALSE;
			}
		if (sorted)
			break;
	}
}

void qsort_rep(struct spectac *pb, struct spectac *pe)
{
	struct spectac *pleft = pb, *pright = pe;
	struct spectac *pm = pb + (pe - pb) / 2;
	while (pleft < pright)
	{
		while (pright->max_tprice >= pm->max_tprice && pleft < pright)
			pright--;
		if (pleft != pright)
		{
			*pleft = *pright;
			pleft++;
		}
		while (pleft->max_tprice <= pm->max_tprice && pleft < pright)
			pleft++;
		if (pleft != pright)
		{
			*pright = *pleft;
			pright--;
		}
	}
	*pleft = *pm;
	pm = pleft;
	pleft = pb;
	pright = pe;
	if (pleft < pm)
		qsort_rep(pleft, pm);
	if (pright > pm)
		qsort_rep(pm + 1, pright);
}

