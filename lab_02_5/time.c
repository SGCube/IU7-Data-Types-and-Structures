#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "theatre.h"
#include "fileio.h"
#include "repert.h"
#include "sort.h"

#define OK 0

typedef unsigned long int tick_t;

tick_t tick(void)
{
	tick_t d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}

int test_set(void)
{
	setbuf(stdin, NULL);
	int rc = OK;
	
	FILE *f[] = { fopen("data/data1.txt", "w"),
		fopen("data/data5.txt", "w"),
		fopen("data/data10.txt", "w"),
		fopen("data/data20.txt", "w"),
		fopen("data/data40.txt", "w"),
		fopen("data/data80.txt", "w"),
		fopen("data/data120.txt", "w"),
		fopen("data/data600.txt", "w") };
	int narr[] = { 1, 5, 10, 20, 40, 80, 120, 600 };
		
	for (int i = 0; i < 8; i++)
		if (!f[i])
		{
			fprintf(stderr, "Couldn't read a file!");
			rc = ERR_FILE;
			break;
		}
	
	printf("  N %17s %17s\n", "Bubble", "Quick sort");
	printf("    %8s %8s %8s %8s\n", "by keys", "by data",
		"by keys", "by data");
	for (int i = 0; i < 8; i++)
	{
		tick_t start, end;
		int n = narr[i];
		struct spectac *data = NULL;
		rc = read_repert(&data, &n, f[i]);
		struct spectac *datar = malloc(n * sizeof(struct spectac));
		for (int j = 0; j < n; j++)
			datar[j] = data[j];
		struct keytable *keys = create_keytable(data, n);
		struct keytable *keysr = create_keytable(data, n);
		printf("%3d ", n);
		start = tick();
		bsort_key(keysr, n);
		end = tick();
		printf("%8lu ", end - start);
		start = tick();
		bsort_rep(datar, n);
		end = tick();
		printf("%8lu ", end - start);
		for (int j = 0; j < n; j++)
		{
			keysr[j] = keys[j];
			datar[j] = data[j];
		}
		start = tick();
		qsort_key(keysr, keysr + n -1);
		end = tick();
		printf("%8lu ", end - start);
		start = tick();
		qsort_rep(datar, datar + n -1);
		end = tick();
		printf("%8lu ", end - start);
		free(data);
		free(datar);
		free(keys);
		free(keysr);
		fclose(f[i]);
	}
	
	return rc;
}