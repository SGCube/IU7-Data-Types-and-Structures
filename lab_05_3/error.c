#include <stdio.h>
#include "error.h"

void errmsg(int rc)
{
	switch (rc)
	{
		case OK:
			break;
		case ERR_MEMORY:
			printf("Queue overflow!\n");
			break;
		case ERR_EMPTY:
			printf("Queue is empty!\n");
			break;
		default:
			printf("Unknown error!\n");
	}
}