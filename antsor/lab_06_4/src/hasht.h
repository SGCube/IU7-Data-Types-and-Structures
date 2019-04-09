#ifndef __HASHT__H__

#define __HASHT__H__

#include <stdio.h>

#define MAX_SIZE 10000
#define MAX_SEARCH 4

typedef struct hash
{
	int value;
	signed char flag;
} hash_t;

void init(hash_t *ht, int n);

int hashf(int key, int p);

int is_prime(int x);

int rehash(hash_t *ht, int *n);

int hread(FILE *f, hash_t *ht, int *n);

int hsearch(int key, hash_t *ht, int n, int *kcmp);

int hremove(int key, hash_t *ht, int n, int *kcmp);

void hprint(hash_t *ht, int n);

#endif