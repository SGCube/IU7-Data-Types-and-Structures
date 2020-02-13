#include <stdio.h>
#include "error.h"
#include "array.h"

int push_arr(char *stack, int *n, char ch)
{
	if (*n == ARR_SIZE)
		return ERR_OVERFLOW;
	stack[*n] = ch;
	*n += 1;
	return OK;
}

char pop_arr(char *stack, int *n)
{
	if (*n == 0)
		return 0;
	*n -= 1;
	return stack[*n];
}

int is_palindrom_arr(char *stack, int *n)
{
	int rc = 1;
	int nn = *n;
	if (nn == 0)
		return -1;
	int m = nn / 2 + nn % 2;
	char tmp1[ARR_SIZE], tmp2[ARR_SIZE];
	char middle = 0;
	int n1 = 0, n2 = 0;
	
	while (*n > m)
	{
		tmp1[n1] = pop_arr(stack, n);
		n1++;
	}
	if (nn % 2 == 1)
		middle = pop_arr(stack, n);
	while (*n > 0 && rc == 1)
	{
		tmp2[n2] = pop_arr(stack, n);
		n2++;
		if (tmp2[n2 - 1] != tmp1[n1 - n2])
			rc = 0;
	}
	
	while (n2 > 0)
		push_arr(stack, n, tmp2[n2-- - 1]);
	if (middle != 0)
		push_arr(stack, n, middle);
	while (n1 > 0)
		push_arr(stack, n, tmp1[n1-- - 1]);
	return rc;
}

void print_arr(char *stack, int *n)
{
	if (*n == 0)
	{
		printf("Stack is empty!\n");
		return;
	}
	char tmp[ARR_SIZE];
	int ntmp = *n;
	while (*n > 0)
	{
		tmp[*n - 1] = pop_arr(stack, n);
		printf("%c ", tmp[*n]);
	}
	while (*n < ntmp)
		push_arr(stack, n, tmp[*n]);
	printf("\n");
}

void clear_arr(char *stack, int *n)
{
	while (*n > 0)
	{
		stack[*n - 1] = 0;
		*n -= 1;
	}
}
