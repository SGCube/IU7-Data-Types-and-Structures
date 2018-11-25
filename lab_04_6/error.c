#include <stdio.h>
#include "error.h"

void errmsg(int rc)
{
	switch (rc)
	{
		case OK:
			break;
		case ERR_INPUT:
			printf("Invalid input!\n");
		case ERR_ALLOC:
			printf("Memory allocation error!\n");
		case ERR_OVERFLOW:
			printf("Stack overflow!\n");
		default:
			printf("Unknown error!\n");
	}
}