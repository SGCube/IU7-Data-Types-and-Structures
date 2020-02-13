#ifndef __ARRAY_T_H__
#define __ARRAY_T_H__

#define ARR_SIZE 1000

typedef struct array_t array;

struct array_t
{
	char data[1000];
	int n;
};

int peek_arr(array arr);
int push_arr(array *arr, char ch);
char pop_arr(array *arr);
void print_arr(array *arr);
int is_palindrom_arr(array *arr);
void clear_arr(array *arr);

#endif