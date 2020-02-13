#include <stdio.h>
#include <math.h>
#include "error.h"
#include "simul.h"

void test_call(FILE *fres, FILE *frep,
	double t1_a, double t1_b, double t2_a, double t2_b)
{
	//init result values
	int nin = 0, nout = 0, noaall = 0;
	double t = 0.0, tavg = 0.0, tdown = 0.0;
	
	//init comparing values
	double t1 = t1_b - t1_a, t2 = t2_b - t2_a;
	double t_in = t1 / 2;
	double t_one = t2 / 2;
	double t_cmp = t_in;
	int n_cmp = nin;
	
	//t1 & t2 print
	fprintf(fres, "%2.1f %2.1f  %2.1f %2.1f\t",
		t1_a, t1_b, t2_a, t2_b);
	fprintf(frep, "%2.1f %2.1f  %2.1f %2.1f\n",
		t1_a, t1_b, t2_a, t2_b);
	
	double k;
	//simulate & read results
	simulate_arr(t1_a, t1_b, t2_a, t2_b);
	FILE *f = fopen("data.txt", "r");
	if (!f)
		return;
	fscanf(f, "%lf %lf %lf %d %d %d", &t, &tavg, &tdown,
		&nin, &nout, &noaall);
	fclose(f);
	//define comparing values
	t_cmp = t_in;
	n_cmp = nin;
	if (t2 > t1)
	{
		t_cmp = t_one;
		n_cmp = noaall;
	}
	//time & n statistic
	fprintf(frep, "Working time: %lf\n", t);
	fprintf(frep, "Elements: in - %d\n", nin);
	fprintf(frep, "OA launch times: %d\n", noaall);
	fprintf(frep, "OA downtime: %lf\n\n", tdown);
	//n fault
	k = t / t_cmp;
	fprintf(fres, "%8.5lf%% ",
		fabs((float)n_cmp - k) / k * 100);
	//time fault	
	k = t_cmp * n_cmp;
	fprintf(fres, "%8.5lf%%\t", fabs(t - k) / k * 100);
	
	//simulate & read results
	simulate_list(t1_a, t1_b, t2_a, t2_b);
	f = fopen("data.txt", "r");
	if (!f)
		return;
	fscanf(f, "%lf %lf %lf %d %d %d", &t, &tavg, &tdown,
		&nin, &nout, &noaall);
	fclose(f);
	//define comparing values
	t_cmp = t_in;
	n_cmp = nin;
	if (t2 > t1)
	{
		t_cmp = t_one;
		n_cmp = noaall;
	}
	//time & n statistic
	fprintf(frep, "Working time: %lf\n", t);
	fprintf(frep, "Elements: in - %d\n", nin);
	fprintf(frep, "OA launch times: %d\n", noaall);
	fprintf(frep, "OA downtime: %lf\n\n", tdown);
	//n fault
	k = t / t_cmp;
	fprintf(fres, "%8.5lf%% ",
		fabs((float)n_cmp - k) / k * 100);
	//time fault
	k = t_cmp * n_cmp;
	fprintf(fres, "%8.5lf%%\n", fabs(t - k) / k * 100);
}

void test()
{
	FILE *fres = fopen("result.txt", "w");
	FILE *frep = fopen("report.txt", "w");
	if (!fres)
		return;
	if (!frep)
		return;
	
	fprintf(fres, "%7s  %7s\t%19s\t%19s\n", "T1", "T2", "array", "list");
	fprintf(fres, "%16s\t%9s %9s\t%9s %9s\n", " ",
		"N Fault", "TimeFault", "N Fault", "TimeFault");
		
	double rng[] = { 0.0, 1.0, 2.0, 4.0, 6.0 };
	
	for (int i = 1; i < sizeof(rng)/sizeof(rng[0]); i++)
		for (int j = 1; j < sizeof(rng)/sizeof(rng[0]); j++)
			test_call(fres, frep, rng[0], rng[i], rng[0], rng[j]);
	fclose(fres);
	fclose(frep);
}