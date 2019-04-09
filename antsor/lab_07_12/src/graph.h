#ifndef __GRAPH__H__
#define __GRAPH__H__

#include <stdio.h>

void init_graph(int *adj_matr, int node_n);

int read_graph(FILE *f, int *adj_matr, int node_n);

void matr_to_dot(FILE *f, const char *name, int *adj, int n, int node_i,
	int *towns);

int *short_dist(int *adj, int n, int node_i);

int *dist_filter(int *d, int n, int value);

#endif