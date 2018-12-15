#ifndef __HASHT__H__

#define __HASHT__H__

#ifdef ARR_EXPORTS
#define ARR_DLL __declspec(dllexport)
#else
#define ARR_DLL __declspec(dllimport)
#endif

#define ARR_DECL __cdecl

#include <stdio.h>

int init(hash_t *ht, int n);

int hashf(int key, int p);

int is_prime(int x);

int rehash(hash_t *ht, int *n);

ARR_DLL int ARR_DECL read(FILE *f, hash_t *ht, int *n)

ARR_DLL int ARR_DECL search(int key, hash_t *ht, int n);

ARR_DLL int ARR_DECL remove(int key, hash_t *ht, int n);

ARR_DLL void ARR_DECL print(hash_t *ht, int n);

#endif