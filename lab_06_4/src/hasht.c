#include <stdio.h>
#include <stdlib.h>
#include "hasht.h"
#include "error.h"

void init(hash_t *ht, int n)
{
	for (int i = 0; i < n; i++)
		ht[i].flag = 0;
}

int hashf(int key, int p)
{
	return key % p;
}

int is_prime(int x)
{
	if (x < 2)
		return 0;
	for (int k = 2; k <= x / 2; k++)
		if (x % k == 0)
			return 0;
	return 1;
}

int rehash(hash_t *ht, int *n)
{
	hash_t *ht_old = malloc(MAX_SIZE * sizeof(hash_t));
	if (!ht_old)
		return ERR_MEMORY;
	for (int i = 0; i < *n; i++)
		ht_old[i] = ht[i];
	int nn = *n;
	*n = *n * 2 + 1;
	while (!is_prime(*n))
		*n += 2;
	if (*n > MAX_SIZE)
	{
		free(ht_old);
		return ERR_MEMORY;
	}
	
	init(ht, *n);
	for (int i = 0; i < nn; i++)
	{
		if (ht_old[i].flag == 1)
		{
			int key = ht_old[i].value;
			int hashval = hashf(key, *n);
			int kcmp = 0;					// количество сравнений
			int j = hashval;				// текущий индекс
			while (kcmp < MAX_SEARCH)
			{
				if (ht[j].flag != 1)	// пустая или удалённая ячейка
				{
					ht[j].value = key;
					ht[j].flag = 1;
					break;
				}
				else if (ht[j].value == key)	// значение уже добавлено
					break;
				else
				{
					j++;
					if (j == *n) // граница выделенной области памяти
						j = 0;
					kcmp++;
				}
				if (kcmp == MAX_SEARCH)
				{
					int sc = rehash(ht, n);
					if (sc != OK)
					{
						free(ht_old);
						return sc;
					}
					kcmp = 0;
					hashval = hashf(key, *n);
					j = hashval;
				}
			}
		}
	}
	free(ht_old);
	return OK;
}

int hread(FILE *f, hash_t *ht, int *n)
{
	if (!f)
		return ERR_FILE;
	if (!ht)
		return ERR_MEMORY;
	if (n <= 0)
		return ERR_NUMB;
	
	int key;
	if (fscanf(f, "%d", &key) == EOF)
		return ERR_EMPTY;
	rewind(f);
	
	init(ht, *n);
	int sc = 1;
	while (sc == 1)
	{
		sc = fscanf(f, "%d", &key);
		if (sc == EOF)
			return OK;
		if (sc != 1)
			return ERR_NUMB;
		int hashval = hashf(key, *n);
		int kcmp = 0;					// количество сравнений
		int i = hashval;				// текущий индекс
		while (kcmp < MAX_SEARCH)
		{
			if (ht[i].flag != 1)	// пустая или удалённая ячейка
			{
				ht[i].value = key;
				ht[i].flag = 1;
				break;
			}
			else if (ht[i].value == key)	// значение уже добавлено
				break;
			else
			{
				i++;
				if (i == *n) // граница выделенной области памяти
					i = 0;
				kcmp++;
			}
			if (kcmp == MAX_SEARCH)
			{
				int rc = rehash(ht, n);
				if (rc != OK)
					return rc;
				kcmp = 0;
				hashval = hashf(key, *n);
				i = hashval;
			}
		}
	}
	return OK;
}

int hsearch(int key, hash_t *ht, int n, int *kcmp)
{
	int hashval = hashf(key, n);		// вычисление значения хэш-функции
	*kcmp = 1;							// количество сравнений
	int i = hashval;					// текущий индекс
	do
	{
		if (ht[i].flag == 0)	// пустая ячейка
			return -1;
		if (ht[i].value == key) // если значение совпадает
			return i;
		i++;
		if (i == n)	// граница выделенной области памяти
			i = 0;
	}
	while (*kcmp < MAX_SEARCH && i != hashval);
	return -1;
}

int hremove(int key, hash_t *ht, int n, int *kcmp)
{
	int ind = hsearch(key, ht, n, kcmp);	//индекс элемента
	if (ind != -1)	//элемент существует
	{
		ht[ind].flag = -1;
		return ind;
	}
	return -1;
}

void hprint(hash_t *ht, int n)
{
	for (int i = 0; i < n; i++)
		if (ht[i].flag == 1)
			printf("%4d ", i);
		else
			printf("---- ");
	printf("\n");
	for (int i = 0; i < n; i++)
		if (ht[i].flag == 1)
			printf("%4d ", ht[i].value);
		else
			printf("---- ");
	printf("\n");
}