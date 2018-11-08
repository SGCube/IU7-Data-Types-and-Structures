#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "matrixio.h"
#include "fileio.h"
#include "error.h"

#define REPEAT 100

typedef unsigned long int tick_t;

tick_t tick(void)
{
	tick_t d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}

int test_time(int rows, int cols, int k, float p)
{
	int rc = OK;
	matrix a = { 0, 0, 0, NULL, NULL, NULL };
	matrix b = { 0, 0, 0, NULL, NULL, NULL };
	rc = matrix_init(&a, rows, cols, k);
	if (rc == OK)
		rc = matrix_init(&b, rows, cols, k);
	if (rc == ERR_ALLOC)
	{
		errmsg(rc);
		free_matrix(a);
		free_matrix(b);
		return ERR_ALLOC;
	}
	if (k > 0)
	{
		a.nk = k;
		b.nk = k;
		matrix_random(&a);
		matrix_random(&b);
	}
	int *astd = matrix_std(a);
	int *bstd = matrix_std(b);
	if (!astd || !bstd)
	{
		errmsg(ERR_ALLOC);
		free_matrix(a);
		free_matrix(b);
		if (astd)
			free(astd);
		if (bstd)
			free(bstd);
		return ERR_ALLOC;
	}
	
	tick_t start, end;
	tick_t times1 = 0, times2 = 0;
	int kres = 0;
	for (int j = 0; j < REPEAT; j++)
	{
		matrix c = { 0, 0, 0, NULL, NULL, NULL };
		rc = matrix_init(&c, rows, cols, k + k);
		if (rc == ERR_ALLOC)
		{
			errmsg(rc);
			free_matrix(a);
			free_matrix(b);
			free_matrix(c);
			return ERR_ALLOC;
		}
		int *cstd = matrix_std_null(rows, cols);
		if (!cstd)
		{
			errmsg(ERR_ALLOC);
			free_matrix(a);
			free_matrix(b);
			free_matrix(c);
			free(astd);
			free(bstd);
			return ERR_ALLOC;
		}
		
		start = tick();
		sum(a, b, &c);
		end = tick();
		times1 += end - start;
		kres = c.nk;
		
		start = tick();
		sum_std(astd, bstd, cstd, rows, cols);
		end = tick();
		times2 += end - start;
		
		free_matrix(c);
		free(cstd);
	}
	
	times1 /= REPEAT;
	times2 /= REPEAT;
	unsigned int mem_sp = (2 * k + rows + 3) * sizeof(int);
	unsigned int mem_sp2 = (2 * kres + rows + 3) * sizeof(int);
	unsigned int mem1 = 2 * mem_sp + mem_sp2;
	unsigned int mem2 = 3 * rows * cols * sizeof(int);
	float time_eff = ((float)times2 - times1) / times2 * 100;
	float mem_eff = ((float)mem2 - mem1) / mem2 * 100;
	
	printf("%4d %4d %7.2f%%\t", rows, cols, p);
	printf("%8lu %8u %8lu %8u ", times1, mem1, times2, mem2);
	printf("%7.2f%% %7.2f%%\n", time_eff, mem_eff);
	
	free_matrix(a);
	free_matrix(b);
	free(astd);
	free(bstd);
	
	return OK;
}

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	srand(time(NULL));
	
	int rc = OK;
	
	printf("%4s %4s %8s\t%17s %17s %17s\n", "N", "M", "nonzero",
		"sparce", "standart", "efficiency");
	printf("%9s %8s\t", " ", " ");
	for (int i = 0; i < 3; i++)
		printf("%8s %8s ", "time", "memory");
	printf("\n");
	
	int sizes[] = { 1, 2, 5, 10, 20, 50, 100, 500, 1000 };
	float perc[] = { 0, 5, 10, 25, 50, 75, 100 };
	
	for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++)
	{
		int rows = sizes[i];
		int cols = sizes[i];
		int kk = -1;
		for (int p = 0; p < sizeof(perc)/sizeof(perc[0]); p++)
		{
			float fk = rows * cols * perc[p] / 100;
			int k = (int)fk;
			if (k > kk)
			{
				kk = k;
				rc = test_time(rows, cols, k, perc[p]);
				if (rc != OK)
				{
					errmsg(rc);
					return rc;
				}
			}
		}
	}
	
	return rc;
}