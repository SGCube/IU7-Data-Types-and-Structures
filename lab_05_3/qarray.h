#ifndef __QARRAY_H__
#define __QARRAY_H__

int push_qarr(int val, int **pin, int **pout, int *k, int *qb, int *qe);

int pop_qarr(int **pout, int *k, int *qb, int *qe);

void print_qarr(int **pin, int **pout, int *k, int *qb, int *qe);

#endif