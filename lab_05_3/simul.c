#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "error.h"
#include "simul.h"
#include "qarray.h"
#include "queue.h"

#define NOUT_END 1000
#define N_PRINT 100
#define P_CONST 0.8

int simulate_arr(double t1_a, double t1_b, double t2_a, double t2_b)
{
	int rc = OK;
	float p = P_CONST;
	float pp;
	
	//t - общее время, tavg - среднее время, tdown - время простоя
	//nin - кол-во входов, nout - кол-во выходов, noaall - OA сработоло
	//noa - OA сработоло % 1000
	//tq - массив интервалов времени
	int nin = 0, nout = 0, noaall = 0;
	int noa = 0;
	double t = 0.0, tavg = 0.0, tdown = 0.0;
	double tmp1 = 0, tmp2 = 0;
	double tq[ARR_SIZE];
	
	int arr[ARR_SIZE], arr_k = 0, arr_kavg = 0;
	int *qb = arr, *qe = arr + ARR_SIZE;
	int *pin = arr, *pout = arr;
	int val1, val2;
	short int is_val = 0, is_new = 1;
	
	while (nout < NOUT_END)
	{
		if (fabs(tmp1) < 1e-12)
			tmp1 = (double) rand() / RAND_MAX * (t1_b - t1_a) + t1_a;
		if (fabs(tmp2) < 1e-12)
			tmp2 = (double) rand() / RAND_MAX * (t2_b - t2_a) + t2_a;
		// push & OA in one time
		if (fabs(tmp1 - tmp2) < 1e-12)
		{
			if (!is_val)
			{
				t += tmp1;
				if (is_new)
					val1 = rand() % 101;
				rc = push_qarr(val1, &pin, &pout, &arr_k, qb, qe);
				if (rc != ERR_MEMORY)
				{
					tq[nin % ARR_SIZE] = t;
					nin++;
				}
			}
			else
			{
				int val3 = val1;
				t += tmp1;
				pp = (float) rand() / RAND_MAX;
				if (pp <= p)
				{
					is_new = 0;
					val1 = val2;
				}
				else
					nout++;
				if (arr_k == 0)
				{
					tdown += tmp2;
					is_val = 0;
				}
				else
				{
					arr_kavg += arr_k;
					val2 = pop_qarr(&pout, &arr_k, qb, qe);
					tavg += t - tq[noa % ARR_SIZE];
					is_val = 1;
					noa++;
				}
				if (is_new)
					val3 = rand() % 101;
				rc = push_qarr(val3, &pin, &pout, &arr_k, qb, qe);
				if (rc != ERR_MEMORY)
				{
					tq[nin % ARR_SIZE] = t;
					nin++;
				}
			}
			if (!is_val)
			{
				arr_kavg += arr_k;
				val2 = pop_qarr(&pout, &arr_k, qb, qe);
				tavg += t - tq[noa % ARR_SIZE];
				noa++;
			}
			is_new = 1;
			is_val = 1;
			tmp1 = 0.0;
			tmp2 = 0.0;
		}
		//push earlier
		else if (tmp1 < tmp2)
		{
			t += tmp1;
			tmp2 -= tmp1;
			if (is_new)
				val1 = rand() % 101;
			rc = push_qarr(val1, &pin, &pout, &arr_k, qb, qe);
			if (rc != ERR_MEMORY)
			{
				tq[nin % ARR_SIZE] = t;
				nin++;
			}
			is_new = 1;
			tmp1 = 0.0;
		}
		//OA earlier
		else if (tmp1 > tmp2)
		{
			tmp1 -= tmp2;
			if (is_val)
			{
				pp = (float) rand() / RAND_MAX;
				if (pp <= p)
				{
					is_new = 0;
					val1 = val2;
				}
				else
					nout++;
			}
			if (arr_k == 0)
			{
				tdown += tmp2;
				is_val = 0;
			}
			else
			{
				arr_kavg += arr_k;
				val2 = pop_qarr(&pout, &arr_k, qb, qe);
				tavg += t - tq[noa % ARR_SIZE];
				is_val = 1;
				noa++;
			}
			t += tmp2;
			tmp2 = 0.0;
		}
		if (noa > 0 && noa % 100 == 0)
		{
			noaall += noa;
			printf("Handled: %d\n", noaall);
			printf("Current length: %d\n", arr_k);
			printf("Average length: %d\n\n", arr_kavg / noaall);
			noa = 0;
		}
	}
	noaall += noa;
	tavg /= noaall;
	printf("Working time: %lf\n", t);
	printf("Average in-queue time: %lf\n", tavg);
	printf("Elements: in - %d, out - %d\n", nin, nout);
	printf("OA launch times: %d\n", noaall);
	printf("OA downtime: %lf\n", tdown);
	
	FILE *f = fopen("data.txt", "w");
	if (!f)
		return -1;
	fprintf(f, "%lf %lf %lf %d %d %d", t, tavg, tdown, nin, nout, noaall);
	fclose(f);
	return 0;
}

int simulate_list(double t1_a, double t1_b, double t2_a, double t2_b)
{
	int rc = OK;
	float p = P_CONST;
	float pp;
	
	int nin = 0, nout = 0, noa = 0, noaall = 0;
	double t = 0.0, tavg = 0.0, tdown = 0.0;
	double tmp1 = 0, tmp2 = 0;
	double tq[NOUT_END];
	
	list *pin = NULL, *pout = NULL;
	int k = 0, kavg = 0;
	int val1, val2;
	short int is_val = 0, is_new = 1;
	
	while (nout < NOUT_END)
	{
		if (fabs(tmp1) < 1e-12)
			tmp1 = (double) rand() / RAND_MAX * (t1_b - t1_a) + t1_a;
		if (fabs(tmp2) < 1e-12)
			tmp2 = (double) rand() / RAND_MAX * (t2_b - t2_a) + t2_a;
		// push & OA in one time
		if (fabs(tmp1 - tmp2) < 1e-12)
		{
			if (!is_val)
			{
				t += tmp1;
				if (is_new)
					val1 = rand() % 101;
				rc = push_queue(val1, &pin, &pout);
				if (rc != ERR_MEMORY)
				{
					tq[nin % NOUT_END] = t;
					nin++;
					k++;
				}
				is_new = 1;
			}
			else
			{
				int val3 = val1;
				t += tmp1;
				pp = (float) rand() / RAND_MAX;
				if (pp <= p)
				{
					is_new = 0;
					val1 = val2;
				}
				else
					nout++;
				if (k == 0)
				{
					tdown += tmp2;
					is_val = 0;
				}
				else
				{
					kavg += k;
					val2 = pop_queue(&pin, &pout);
					tavg += t - tq[noa % NOUT_END];
					is_val = 1;
					noa++;
					k--;
				}
				if (is_new)
					val3 = rand() % 101;
				rc = push_queue(val3, &pin, &pout);
				if (rc != ERR_MEMORY)
				{
					tq[nin % NOUT_END] = t;
					nin++;
					k++;
				}
			}
			if (!is_val)
			{
				kavg += k;
				val2 = pop_queue(&pin, &pout);
				tavg += t - tq[noa % NOUT_END];
				noa++;
				k--;
			}
			is_new = 1;
			is_val = 1;
			tmp1 = 0.0;
			tmp2 = 0.0;
		}
		//push earlier	
		else if (tmp1 < tmp2)
		{
			t += tmp1;
			tmp2 -= tmp1;
			if (is_new)
				val1 = rand() % 101;
			rc = push_queue(val1, &pin, &pout);
			if (rc != ERR_MEMORY)
			{
				tq[nin % NOUT_END] = t;
				nin++;
				k++;
			}
			is_new = 1;
			tmp1 = 0.0;
		}
		//OA earlier
		else if (tmp1 > tmp2)
		{
			tmp1 -= tmp2;
			if (is_val)
			{
				pp = (float) rand() / RAND_MAX;
				if (pp <= p)
				{
					is_new = 0;
					val1 = val2;
				}
				else
					nout++;
			}
			if (k == 0)
			{
				tdown += tmp2;
				is_val = 0;
			}
			else
			{
				kavg += k;
				val2 = pop_queue(&pin, &pout);
				tavg += t - tq[noa % NOUT_END];
				is_val = 1;
				noa++;
				k--;
			}
			t += tmp2;
			tmp2 = 0.0;
		}
		if (noa > 0 && noa % 100 == 0)
		{
			noaall += noa;
			printf("Handled: %d\n", noaall);
			printf("Current length: %d\n", k);
			printf("Average length: %d\n\n", kavg / noaall);
			noa = 0;
		}
	}
	noaall += noa;
	tavg /= noaall;
	printf("Working time: %lf\n", t);
	printf("Average in-queue time: %lf\n", tavg);
	printf("Elements: in - %d, out - %d\n", nin, nout);
	printf("OA launch times: %d\n", noaall);
	printf("OA downtime: %lf\n", tdown);
	
	FILE *f = fopen("data.txt", "w");
	if (!f)
		return -1;
	fprintf(f, "%lf %lf %lf %d %d %d", t, tavg, tdown, nin, nout, noaall);
	fclose(f);
	
	return 0;
}