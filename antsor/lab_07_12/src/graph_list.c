#include <stdlib.h>
#include <assert.h>
#include "graph_list.h"
#include "error.h"

void init_lgraph(node_t *adj[], int node_n)
{
	assert(adj && node_n > 0);
	for (int i = 0; i < node_n; i++)
		adj[i] = NULL;
}

int read_lgraph(FILE *f, node_t *adj[], int node_n)
{
	assert(f && adj && node_n > 0);
	int i = 0, j = 0, k = 0, w = 0;
	while (!feof(f))
	{
		int sc = fscanf(f, "%d%d%d", &i, &j, &w);
		if (sc == EOF)
			return OK;
		if (sc < 3 || w < 0)
			return ERR_DATA;
		if (i <= 0 || i > node_n || j <= 0 || j > node_n)
			return ERR_DATA;
		node_t *node = create_node(j - 1, w);
		if (!node)
			return ERR_MEMORY;
		
		adj[i - 1] = push_back(adj[i - 1], node);
		node = create_node(i - 1, w);
		if (!node)
			return ERR_MEMORY;
		adj[j - 1] = push_back(adj[j - 1], node);
		k++;
	}
	return OK;
}

void list_to_dot(FILE *f, const char *name, node_t *adj[], int n,
	int node_i, int *towns)
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
	{
		node_t *cur = adj[i];
		while (cur)
		{
			if (cur->no > i)
				fprintf(f, "%d -- %d [label=%d];\n",
						i + 1, cur->no + 1, cur->w);
			cur = cur->next;
		}
	}
    fprintf(f, "}\n");	
}

int *short_ldist(node_t *adj[], int n, int node_i)
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
		if (imin != -1)
		{
			node_t *cur = adj[imin];
			while (cur)
			{
				int tmp = min + cur->w;
				if (tmp < d[cur->no] || d[cur->no] == -1)
					d[cur->no] = tmp;
				cur = cur->next;
			}
			v[imin] = 1;
		}
	}
	while (imin != -1);
	free(v);
	return d;
}