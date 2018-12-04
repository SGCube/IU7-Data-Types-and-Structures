#include <stdio.h>
#include "qarray.h"
#include "error.h"

int push_qarr(int val, int **pin, int **pout, int *k, int *qb, int *qe)
{
	if (*pin == *pout && *k != 0)
		return ERR_MEMORY;
	**pin = val;
	*pin += 1;
	if (*pin == qe)
		*pin = qb;
	*k += 1;
	return OK;
}

int pop_qarr(int **pout, int *k, int *qb, int *qe)
{
	if (*k == 0)
		return ERR_EMPTY;
	int rc = **pout;
	*pout += 1;
	if (*pout == qe)
		*pout = qb;
	*k -= 1;
	return rc;
}

void print_qarr(int **pin, int **pout, int *k, int *qb, int *qe)
{
	if (*k == 0)
	{
		printf("Queue is empty!\n");
		return;
	}
	
	int tmp[ARR_SIZE], tk = 0;
	int *tqb = tmp, *tqe = tmp + ARR_SIZE;
	int *tpin = tqb, *tpout = tqb;
	
	while (*k > 0)
	{
		int tmp = pop_qarr(pout, k, qb, qe);
		printf("%d ", tmp);
		push_qarr(tmp, &tpin, &tpout, &tk, tqb, tqe);
	}
	while (tk > 0)
		push_qarr(pop_qarr(&tpout, &tk, tqb, tqe),
			pin, pout, k, qb, qe);
}