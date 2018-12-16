#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "file.h"

#define BUF_SIZE 20

ARR_DLL int ARR_DECL fsearch(FILE *f, int numb)
{
	if (!f)
		return ERR_FILE;

	int x;
	if (fscanf(f, "%d", &x) == EOF)
		return ERR_EMPTY;
	rewind(f);
	
	int k = 0;
	while (fscanf(f, "%d", &x) == 1)
	{
		if (numb == x)
			return k;
		k++;
	}
	return NOT_FOUND;
}

ARR_DLL int ARR_DECL fremove(FILE *f, int numb)
{
	int ind = fsearch(f, numb);
	rewind(f);
	if (ind >= 0)
	{
		rewind(f);
	}
	return OK;
}