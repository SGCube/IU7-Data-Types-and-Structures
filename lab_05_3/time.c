#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"
#include "qarray.h"
#include "simul.h"
#include "error.h"

#define ARR_SIZE_BIG 1000

typedef unsigned long int tick_t;

tick_t tick(void)
{
	tick_t d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}

int time_measure(FILE *f, int len)
{
	int rc = OK;
	tick_t start = 0, end = 0;
	
	tick_t tpush_list = 0, tpop_list = 0, tsim_list = 0;
	tick_t tpush_arr = 0, tpop_arr = 0, tsim_arr = 0;
	
	for (int j = 0; j < 100; j++)
	{
		list *pin = NULL, *pout = NULL;
		for (int k = 0; k < len; k++)
		{
			int d = rand() % 100 - 50;
			start = tick();
			rc = push_queue(d, &pin, &pout);
			end = tick();
			if (rc != OK)
				return rc;
			tpush_list += end - start;
		}
		
		start = tick();
		simulate_list(1.0, 4.0, 1.0, 4.0);
		end = tick();
		tsim_list += end - start;
		for (int k = 0; k < len; k++)
		{
			start = tick();
			pop_queue(&pin, &pout);
			end = tick();
			tpop_list += end - start;
		}
	}
	tpush_list /= 100;
	tsim_list /= 100;
	tpop_list /= 100;
	fprintf(f, "%6lu %6lu %6lu ", tpush_list, tpop_list, tsim_list);
	
	for (int j = 0; j < 100; j++)
	{
		int arr[ARR_SIZE_BIG], arr_k = 0;
		int *qb = arr, *qe = arr + ARR_SIZE_BIG;
		int *pin = arr, *pout = arr;
		for (int k = 0; k < len; k++)
		{
			int d = rand() % 100 - 50;
			start = tick();
			rc = push_qarr(d, &pin, &pout, &arr_k, qb, qe);
			end = tick();
			if (rc != OK)
				return rc;
			tpush_arr += end - start;
		}
		
		start = tick();
		simulate_arr(1.0, 4.0, 1.0, 4.0);
		end = tick();
		tsim_arr += end - start;
		
		for (int k = 0; k < len; k++)
		{
			start = tick();
			pop_qarr(&pout, &arr_k, qb, qe);
			end = tick();
			tpop_arr += end - start;
		}
	}
	tpush_arr /= 100;
	tpop_arr /= 100;
	tsim_arr /= 100;
	fprintf(f, "%6lu %6lu %6lu ", tpush_arr, tpop_arr, tsim_arr);
	
	size_t mem_list = len * sizeof(list);
	size_t mem_arr = len * sizeof(char) + sizeof(char *);
	float tpush_eff = ((float)tpush_arr - tpush_list) / tpush_arr * 100;
	float tpop_eff = ((float)tpop_arr - tpop_list) / tpop_arr * 100;
	float tsim_eff = ((float)tsim_arr - tsim_list) / tsim_arr * 100;
	float mem_eff = ((float)mem_arr - mem_list) / mem_arr * 100;
	fprintf(f, "%7.2f%% %7.2f%% %7.2f%% %7.2f%%\n",
		tpush_eff, tpop_eff, tsim_eff, mem_eff);
	
	return rc;
}

int main()
{
	FILE *f = fopen("time.txt", "w");
	if (!f)
		return -1;
	srand(time(NULL));
	int rc = OK;
	setbuf(stdout, NULL);
	int arrk[] = { 1, 5, 10, 20, 40, 75, 100 };
	fprintf(f, "%4s %22s %22s %26s %s\n", "Len", "Stack by list",
		"Stack by array", "Time eff", "Memory eff");
	fprintf(f, "%4s %6s %6s %8s %6s %6s %8s %8s %8s %8s\n\n", " ",
		"push", "pop", "simul",
		"push", "pop", "simul",
		"push", "pop", "simul");
	for (int i = 0; i < sizeof(arrk)/sizeof(arrk[0]); i++)
	{
		fprintf(f, "%4d ", arrk[i]);
		rc = time_measure(f, arrk[i]);
		if (rc != OK)
		{
			errmsg(rc);
			return rc;
		}
	}
	fclose(f);
	return rc;
}	