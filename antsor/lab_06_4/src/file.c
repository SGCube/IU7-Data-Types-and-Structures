#include <stdlib.h>
#include "error.h"
#include "file.h"

#define MAX_SIZE 1000

int fsearch(FILE *f, int numb, int *kcmp)
{
	int x, k = 0;
	do 
	{
		fscanf(f, "%d", &x);
		if (x == numb)
		{
			*kcmp = k;
			return k;
		}
		k++;
	}
	while (!feof(f));
	*kcmp = k - 1;
	return NOT_FOUND;
}

int fremove(char *fname, int numb, int *kcmp)
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
	*kcmp = ind;
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