#ifndef __ARRAY_H__
#define __ARRAY_H__

typedef struct array_t array;

struct array_t
{
	char data[ARR_SIZE];
	char *peak;
};

int peek_arr(array arr);
int push_arr(array *arr, char ch);
void pop_arr(array *arr);
void print_arr(array arr);
int is_palindrom_arr(array arr);

#endif