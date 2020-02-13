#include <stdio.h>
#include "error.h"

void errmsg(int rc)
{
	switch (rc)
	{
		case OK:
			break;
		case ERR_INPUT:
			printf("Input error!\n");
			break;
		case ERR_FILE:
			printf("File open error!\n");
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
		case ERR_INDEX:
			printf("Invalid matrix index!\n");
			break;
		case ERR_ALLOC:
			printf("Memory allocation error!\n");
			break;
		case ERR_MISSED_DATA:
			printf("Not enough data!\n");
			break;
		case ERR_AMOUNT:
			printf("Invalid amount!\n");
			break;
		case ERR_ZERO:
			printf("Zero element!\n");
			break;
		case ERR_DIFF_SIZE:
			printf("Matrices have different sizes!\n");
			break;
		default:
			printf("Unknown error!\n");
			break;
	}
}