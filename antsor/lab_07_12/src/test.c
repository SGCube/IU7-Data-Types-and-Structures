#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "error.h"
#include "matrix.h"
#include "graph.h"
#include "list.h"
#include "graph_list.h"

typedef unsigned long int tick_t;

tick_t tick(void)
{
	tick_t d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}

void random_graph(int *adj, int n)
{
	for (int i = 0; i < n; i++)
	{
		adj[i * n + i] = 0;
		for (int j = i + 1; j < n; j++)
		{
			int x = rand() % 2;
			if (x == 0)
			{
				adj[i * n + j] = -1;
				adj[j * n + i] = -1;
			}
			else
			{
				x = rand() % 20;
				adj[i * n + j] = x;
				adj[j * n + i] = x;
			}
		}
	}
}

node_t **list_graph(int *adj_matr, int n, int *k)
{
	assert(adj_matr && n > 0 && k);
	node_t **adj = calloc(n, sizeof(node_t*));
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (adj_matr[i * n + j] > 0)
			{
				node_t *node = create_node(j, adj_matr[i * n + j]);
				if (!node)
				{
					free_all(adj, i);
					return NULL;
				}
				adj[i] = push_back(adj[i], node);
				*k += 1;
			}
	return adj;
}

int main()
{
	srand(time(NULL));
	int *adj_matr = NULL;		//матрица смежности графа
	int *d = NULL;				//массивы кратчайших расстояний
	int *dl = NULL;
	
	node_t **adj_list = NULL;	//список смежностей
	
	int s[] = { 5, 10, 25, 50, 100 }; 
	
	tick_t time_matr[5] = { 0, 0, 0, 0, 0 };
	tick_t time_list[5] = { 0, 0, 0, 0, 0 };
	int v[] = { 0, 0, 0, 0, 0 };	//количество рёбер
	tick_t start = 0, end = 0, res = 0, res2 = 0;
	
	for (int i = 0; i < sizeof(s)/sizeof(s[0]); i++)
	{
		adj_matr = malloc(s[i] * s[i] * sizeof(int));
		if (!adj_matr)
			return ERR_MEMORY;
		random_graph(adj_matr, s[i]);
		v[i] = 0;
		adj_list = list_graph(adj_matr, s[i], &v[i]);
		if (!adj_list)
		{
			free(adj_matr);
			return ERR_MEMORY;
		}
		res = 0;
		res2 = 0;
		for (int j = 0; j < s[i]; j++)
		{
			start = tick();
			d = short_dist(adj_matr, s[i], j);
			end = tick();
			res += end - start;
			start = tick();
			dl = short_ldist(adj_list, s[i], j);
			end = tick();
			res2 += end - start;
			free(d);
			free(dl);
			d = NULL;
			dl = NULL;
		}
		time_matr[i] += res / s[i];
		time_list[i] += res2 / s[i];
	}
	
	printf("%3s\t%12s\t%12s\n", "N", "matrix", "list");
		
	for (int i = 0; i < sizeof(s)/sizeof(s[0]); i++)
	{
		printf("%3d\t%12lu\t%12lu\n", s[i], time_matr[i], time_list[i]);
		printf("\t%8u byt\t%8u byt\n",
			(unsigned) (s[i] * s[i] * sizeof(int)),
			(unsigned) (v[i] * sizeof(node_t) + s[i] * sizeof(node_t *)));
	}
	return OK;
}