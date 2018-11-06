#include <stdio.h>
#include "error.h"

void errmsg(int rc)
{
	switch (rc)
	{
		case ERR_INPUT:
			printf("Input error!\n");
			break;
		case ERR_EMPTY:
			printf("Empty file!\n");
			break;
		case ERR_SIZE:
			printf("Invalid matrix size!\n");
			break;
		case ERR_MATRIX:
			printf("Invalid matrix element!\n");
			break;
		case ERR_ALLOC:
			printf("Memory allocation error!\n");
			break;
		case ERR_MISSED_DATA:
			printf("Not enough data!\n");
			break;
	}
}