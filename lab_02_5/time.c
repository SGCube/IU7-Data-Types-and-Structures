#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "theatre.h"
#inclide "fileio.h"
#include "sort.h"

#define OK 0

typedef unsigned long int tick_t;

tick_t tick(void)
{
	tick_t d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}

void time_write(FILE *f, int *pa, int *pb, int n)
{
	assert(f);
	assert(pa && pb);
	assert(n > 0);
	
	tick_t t1, t2;
	
	fprintf(f, "%u\t", n);
	t1 = tick();
	mysort(pa, n, sizeof(int), compar_int);
	t2 = tick();
	fprintf(f, "%lu\t", t2 - t1);
	
	t1 = tick();
	qsort(pb, n, sizeof(int), compar_int);
	t2 = tick();
	fprintf(f, "%lu\n", t2 - t1);
}

int main()
{
	setbuf(stdin, NULL);
	int rc = OK;
	
	FILE *f[] = { fopen("rep1.txt", "w"),
		fopen("rep5.txt", "w"),
		fopen("rep10.txt", "w"),
		fopen("rep20.txt", "w"),
		fopen("rep40.txt", "w"),
		fopen("rep80.txt", "w"),
		fopen("rep120.txt", "w"),
		fopen("rep600.txt", "w") };
		
	int (*t_func[3])(int **, int **, int) = {
		time_sorted,
		time_random,
		time_inverted };
		
	for (int i = 0; i < 3; i++)
		if (!f[i])
		{
			fprintf(stderr, "Couldn't create output file!");
			rc = ERR_FILE;
			break;
		}
	
	for (int j = 0; j < 3; j++)
	{
		for (int i = 1; i <= N; i++)
		{
			rc = t_func[j](&pa, &pb, i);
			if (rc == OK)
			{
				time_write(f[j], pa, pb, i);
				free(pa);
				free(pb);
			}
			else
				fprintf(stderr, "Memory allocation error!\n");
		}
	}
	
	for (int i = 0; i < 3; i++)
		if (f[i])
			fclose(f[i]);
	
	return rc;
}