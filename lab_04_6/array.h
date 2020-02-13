#ifndef __ARRAY_H__
#define __ARRAY_H__

#define ARR_SIZE 1000

int push_arr(char *stack, int *n, char ch);
char pop_arr(char *stack, int *n);
void print_arr(char *stack, int *n);
int is_palindrom_arr(char *stack, int *n);
void clear_arr(char *stack, int *n);

#endif