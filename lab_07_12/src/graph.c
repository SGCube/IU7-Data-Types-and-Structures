#include <stdlib.h>
#include <assert.h>
#include "graph.h"
#include "error.h"

void init_graph(int *adj_matr, int node_n)
{
	assert(adj_matr && node_n > 0);
	for (int i = 0; i < node_n; i++)
	{	
		for (int j = 0; j < i; j++)
			adj_matr[i * node_n + j] = -1;
		adj_matr[i * node_n + i] = 0;
		for (int j = i + 1; j < node_n; j++)
			adj_matr[i * node_n + j] = -1;
	}
}


int read_graph(FILE *f, int *adj_matr, int node_n)
{
	assert(f && adj_matr && node_n > 0);
	int i = 0, j = 0, k = 0, w = 0;
	int nsq = node_n * node_n / 2 + 1;
	while (!feof(f) && k < nsq)
	{
		int sc = fscanf(f, "%d%d%d", &i, &j, &w);
		if (sc == EOF)
			return OK;
		if (sc < 3 || w < 0)
			return ERR_DATA;
		if (i < 0 || i > node_n || j < 0 || j > node_n)
			return ERR_DATA;
		if (adj_matr[(i - 1) * node_n + (j - 1)] > 0)
			return ERR_DATA;
		adj_matr[(i - 1) * node_n + (j - 1)] = w;
		adj_matr[(j - 1) * node_n + (i - 1)] = w;
		k++;
	}
	return OK;
}

void matr_to_dot(FILE *f, const char *name, int *adj, int n, int node_i,
	int *towns)
{
	assert(f && name && adj);
	
    fprintf(f, "graph %s {\n", name);
	for (int i = 0; i < n; i++)
	{
		fprintf(f, "%d", i + 1);
		if (i + 1 == node_i)
			fprintf(f, " [style=filled, fillcolor=red]");
		else if (towns[i])
			fprintf(f, " [style=filled, fillcolor=green]");
		fprintf(f, ";\n");
	}
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (adj[i * n + j] > 0)
				fprintf(f, "%d -- %d [label=%d];\n",
					i + 1, j + 1, adj[i * n + j]);

    fprintf(f, "}\n");
}

int *short_dist(int *adj, int n, int node_i)
{
	int *d = malloc(n * sizeof(int));
	if (!d)
		return NULL;
	int *v = calloc(n, sizeof(int));
	if (!v)
		return NULL;
	
	for (int i = 0; i < n; i++)
		d[i] = -1;
	d[node_i] = 0;
	
	int imin = -1, min = -1;
	do
	{
		imin = -1;
		min = -1;
		for (int i = 0; i < n; i++)
			if (v[i] == 0 && (d[i] < min || min == -1) && d[i] != -1)
			{
				min = d[i];
				imin = i;
			}
		printf("%d - %d\t", imin, min);
		if (imin != -1)
		{
			for (int i = 0; i < n; i++)
				if (adj[imin * n + i] > 0)
				{
					int tmp = min + adj[imin * n + i];
					if (tmp < d[i] || d[i] == -1)
						d[i] = tmp;
				}
			v[imin] = 1;
		}
		
		for (int i = 0; i < n; i++)
		if (d[i] >= 0)
			printf("%d ", d[i]);
		else
			printf("- ");
		printf("\n");
		
	}
	while (imin != -1);
	free(v);
	return d;
}

int *dist_filter(int *d, int n, int value)
{
	int *t = calloc(n, sizeof(int));
	if (!t)
		return NULL;
	for (int i = 0; i < n; i++)
		if (d[i] > value)
			t[i] = 1;
	return t;
}