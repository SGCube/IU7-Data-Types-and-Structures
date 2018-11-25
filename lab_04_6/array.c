#include <stdio.h>
#include "error.h"
#include "array.h"

int push_arr(array *arr, char ch)
{
	if (!arr->peak)
		arr->peak = arr->data;
	else if (arr->peak - arr->data < ARR_SIZE)
		arr->peak += 1;
	else
		return ERR_OVERFLOW;
	*(arr->peak) = ch;
	return OK;
}

void pop_arr(array *arr)
{
	*(arr->peak) = 0;
	if (arr->peak == arr->data)
		arr->peak = NULL;
	else
		arr->peak -= 1;
}

int peek_arr(array arr)
{
	if (!arr.peak)
		return 0;
	return arr.peak - arr.data + 1;
}

int is_palindrom_arr(array arr)
{
	int len = peek_arr(arr);
	if (len == 0)
		return -1;
	char *tmp1 = arr.peak;
	for (int k = 0; k < len / 2; k++)
	{
		char *tmp2 = tmp1;
		for (int j = k; j < len - k - 1; j++, tmp2--);
		if (*tmp1 != *tmp2)
			return 0;
		tmp1--;
	}
	return 1;
}

void print_arr(array arr)
{
	if (!arr.peak)
	{
		printf("Stack is empty!\n");
		return;
	}
	char *tmp = arr.peak;
	do
	{
		printf("%c ", *tmp);
		tmp -= 1;
	}
	while(tmp >= arr.data);
	printf("\n");
}
