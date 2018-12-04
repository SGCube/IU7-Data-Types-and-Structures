#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "error.h"
#include "qarray.h"
#include "queue.h"
#include "simul.h"
#include "test.h"

#define T1_A 0.0
#define T1_B 6.0
#define T2_A 0.0
#define T2_B 1.0

int main()
{
	double t1_a = T1_A, t1_b = T1_B;
	double t2_a = T2_A, t2_b = T2_B;
	
	srand(time(NULL));
	int action = -1;
	setbuf(stdout, NULL);
	printf("Enter your choice: 0 - array, 1 - list, 2 - test\n");
	int rc = scanf("%d", &action);
	if (rc != 1 || action > 2 || action < 0)
		printf("Error action!\n");
	else if (action == 0)
	{
		printf("Enter T1 & T2 intervals: ");
		if (scanf("%lf%lf%lf%lf", &t1_a, &t1_b, &t2_a, &t2_b) != 4)
		{
			printf("Invalid input! Used standart values\n"
				"T1 = {%2.1f ; %2.1f}, T2 = {%2.1f ; %2.1f}\n",
				T1_A, T1_B, T2_A, T2_B);
			t1_a = T1_A;
			t1_b = T1_B;
			t2_a = T2_A;
			t2_b = T2_B;
		}
		simulate_arr(t1_a, t1_b, t2_a, t2_b);
	}
	else if (action == 1)
	{
		printf("Enter T1 & T2 intervals: ");
		if (scanf("%lf%lf%lf%lf", &t1_a, &t1_b, &t2_a, &t2_b) != 4)
		{
			printf("Invalid input! Used standart values\n"
				"T1 = {%2.1f ; %2.1f}, T2 = {%2.1f ; %2.1f}\n",
				T1_A, T1_B, T2_A, T2_B);
			t1_a = T1_A;
			t1_b = T1_B;
			t2_a = T2_A;
			t2_b = T2_B;
		}
		simulate_list(t1_a, t1_b, t2_a, t2_b);
	}
	else if (action == 2)
		test();
	
	return 0;
}