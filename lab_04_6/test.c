#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "error.h"
#include "stack.h"
#include "array.h"

typedef unsigned long int tick_t;

tick_t tick(void)
{
	tick_t d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}

int time_measure(int len)
{
	int rc = OK;
	tick_t start = 0, end = 0;
	
	tick_t tpush_list = 0, tpop_list = 0, tpal_list = 0;
	tick_t tpush_arr = 0, tpop_arr = 0, tpal_arr = 0;
	
	for (int j = 0; j < 1000; j++)
	{
		list *stack = NULL;
		//frspc *space = NULL;
		for (int k = 0; k < len; k++)
		{
			char ch = rand() % 4 + 'a';
			start = tick();
			rc = push_new(&stack, ch);
			end = tick();
			if (rc != OK)
				return rc;
			tpush_list += end - start;
		}
		
		start = tick();
		is_palindrom(stack);
		end = tick();
		tpal_list += end - start;
		
		for (int k = 0; k < len; k++)
		{
			start = tick();
			pop_free(&stack);
			end = tick();
			/*if (rc != OK)
				return rc;*/
			tpop_list += end - start;
		}
		//free_spaces(&space);
	}
	tpush_list /= 1000;
	tpal_list /= 1000;
	tpop_list /= 1000;
	printf("%6lu %6lu %6lu ", tpush_list, tpop_list, tpal_list);
	
	for (int j = 0; j < 1000; j++)
	{
		array arr = { .peak = NULL };
		for (int k = 0; k < len; k++)
		{
			char ch = rand() % 4 + 'a';
			start = tick();
			rc = push_arr(&arr, ch);
			end = tick();
			if (rc != OK)
				return rc;
			tpush_arr += end - start;
		}
		
		start = tick();
		is_palindrom_arr(arr);
		end = tick();
		tpal_arr += end - start;
		
		for (int k = 0; k < len; k++)
		{
			start = tick();
			pop_arr(&arr);
			end = tick();
			tpop_arr += end - start;
		}
	}
	tpush_arr /= 1000;
	tpop_arr /= 1000;
	tpal_arr /= 1000;
	printf("%6lu %6lu %6lu ", tpush_arr, tpop_arr, tpal_arr);
	
	size_t mem_list = len * sizeof(list);
	size_t mem_arr = len * sizeof(char) + sizeof(char *);
	float tpush_eff = ((float)tpush_arr - tpush_list) / tpush_arr * 100;
	float tpop_eff = ((float)tpop_arr - tpop_list) / tpop_arr * 100;
	float tpal_eff = ((float)tpal_arr - tpal_list) / tpal_arr * 100;
	float mem_eff = ((float)mem_arr - mem_list) / mem_arr * 100;
	printf("%6.2f%% %7.2f%% %7.2f%% %7.2f%%\n",
		tpush_eff, tpop_eff, tpal_eff, mem_eff);
	
	return rc;
}

int main()
{
	srand(time(NULL));
	int rc = OK;
	setbuf(stdout, NULL);
	int arrk[] = { 1, 5, 10, 20, 40, 75, 100, 250, 500, 750, ARR_SIZE };
	printf("%4s %20s %20s %26s %s\n", "Len", "Stack by list",
		"Stack by array", "Time eff", "Memory eff");
	printf("%4s %6s %6s %6s %6s %6s %6s %8s %8s %8s\n\n", " ",
		"push", "pop", "palind", "push", "pop", "palind",
		"push", "pop", "palind");
	for (int i = 0; i < sizeof(arrk)/sizeof(arrk[0]); i++)
	{
		printf("%4d ", arrk[i]);
		rc = time_measure(arrk[i]);
		if (rc != OK)
		{
			errmsg(rc);
			return rc;
		}
	}
	return rc;
}	