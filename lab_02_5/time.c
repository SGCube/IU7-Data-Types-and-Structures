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
	
	FILE *f[] = { fopen("data/data1.txt", "r"),
		fopen("data/data5.txt", "r"),
		fopen("data/data10.txt", "r"),
		fopen("data/data40.txt", "r"),
		fopen("data/data120.txt", "r"),
		fopen("data/data600.txt", "r") };
		
	for (int i = 0; i < 6; i++)
		if (!f[i])
		{
			fprintf(stderr, "Couldn't read a file!");
			rc = ERR_FILE;
			break;
		}
	
	printf("  N %17s %17s %17s\n", "Bubble", "Quick sort", "Memory");
	printf("    %8s %8s %8s %8s %8s %8s\n", "by keys", "by data",
		"by keys", "by data", "by keys", "by data");
	
	for (int i = 0; i < 6; i++)
	{
		tick_t start, end;
		int n = 0;
		tick_t times[4] = { 0, 0, 0, 0 };
		for (int j = 0; j < 100; j++)
		{
			struct spectac *data = malloc(RECORD_N * sizeof(struct spectac));
			rc = read_repert(&data, &n, f[i]);
			struct keytable *keys = create_keytable(data, n);
			
			start = tick();
			bsort_key(keys, n);
			end = tick();
			times[0] += end - start;
			
			start = tick();
			bsort_rep(data, n);
			end = tick();
			times[1] += end - start;
			
			rewind(f[i]);
			rc = read_repert(&data, &n, f[i]);
			free(keys);
			keys = create_keytable(data, n); 
			
			start = tick();
			qsort_key(keys, keys + n - 1);
			end = tick();
			times[2] += end - start;
			
			start = tick();
			qsort_rep(data, data + n - 1);
			end = tick();
			times[3] += end - start;
			
			free(data);
			free(keys);
		}
		for (int j = 0; j < 3; j++)
			times[j] /= 100;
		printf("%3d ", n);
		for (int j = 0; j < 3; j++)
			printf("%8lu ", times[j]);
		unsigned int kmem = n * sizeof(struct keytable);
		unsigned int tmem = n * sizeof(struct spectac);
		printf("%8u %8u %.2f%%\n", kmem, tmem, (float )kmem / tmem * 100);
		fclose(f[i]);
	}
	
	return rc;
}