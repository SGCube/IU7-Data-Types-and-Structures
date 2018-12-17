#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "file.h"

#define MAX_SIZE 1000

ARR_DLL int ARR_DECL fsearch(FILE *f, int numb)
{
	int x, k = 0;
	do 
	{
		fscanf(f, "%d", &x);
		if (x == numb)
			return k;
		k++;
	}
	while (!feof(f));
	return NOT_FOUND;
}

ARR_DLL int ARR_DECL fremove(char *fname, int numb)
{
	int *arr = malloc(MAX_SIZE * sizeof(int));
	if (!arr)
		return ERR_MEMORY;
	
	FILE *f = fopen(fname, "r");
	if (!f)
		return ERR_FILE;
	
	short int found = 0;
	int ind = 0, n = 0;
	while (found == 0 && fscanf(f, "%d", &arr[n]) == 1)
	{
		if (numb == arr[n])
			found = 1;
		n++;
		ind++;
	}
	while (fscanf(f, "%d", &arr[n]) == 1)
		n++;
	fclose(f);
	
	if (ind >= 0)
	{
		FILE *g = fopen(fname, "w");
		for (int i = 0; i < ind; i++)
			fprintf(f, "%d ", arr[i]);
		for (int i = ind + 1; i < n; i++)
			fprintf(f, "%d ", arr[i]);
		fclose(g);
	}
	return OK;
}