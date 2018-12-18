#ifndef __GRAPH__LIST__H__
#define __GRAPH__LIST__H__

#include <stdio.h>

void init_lgraph(node_t *adj[], int node_n);

int read_lgraph(FILE *f, node_t *adj[], int node_n);

void list_to_dot(FILE *f, const char *name, node_t *adj[], int n,
	int node_i, int *towns);

int *short_ldist(node_t *adj[], int n, int node_i);

#endif