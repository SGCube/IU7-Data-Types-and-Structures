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
			break;
		case ERR_ALLOC:
			printf("Memory allocation error!\n");
			break;
		case ERR_OVERFLOW:
			printf("Stack overflow!\n");
			break;
		default:
			printf("Unknown error!\n");
	}
}