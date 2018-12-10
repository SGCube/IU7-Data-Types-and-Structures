#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "file.h"

#define BUF_SIZE 20

ARR_DLL int ARR_DECL fsearch(FILE *f, int numb, int *rc)
{
	if (!f)
	{
		*rc = ERR_FILE;
		return ERR_FILE;
	}
	
	*rc = OK;
	int x;
	if (fscanf(f, "%d", &x) == EOF)
	{
		*rc = ERR_EMPTY;
		return ERR_EMPTY;
	}
	rewind(f);
	
	short int found = 0;
	
	while (fscanf(f, "%d", &x) == 1 && !found)
		if (numb == x)
			found = 1;
		
	if (found)
		return numb;
	*rc = NOT_FOUND;
	return 0;
}