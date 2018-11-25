#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "stack.h"
#include "array.h"

int main()
{
	int rc = OK;
	char act = 0;
	list *stack = NULL;
	frspc *space = NULL;
	array arr = { .peak = NULL };
	setbuf(stdout, NULL);
	do
	{
		fflush(stdin);
		printf("\nMENU:\n"
			"1 - stack state (dynamic list)\n"
			"2 - add symbol to stack (dynamic list)\n"
			"3 - add string to stack (dynamic list)\n"
			"4 - remove symbol from stack (dynamic list)\n"
			"5 - stack state (static array)\n"
			"6 - add symbol to stack (static array)\n"
			"7 - add string to stack (static array)\n"
			"8 - remove symbol from stack (static array)\n"
			"other - exit\n"
			"Enter action: ");
		scanf("%c", &act);
		printf("\n");
		if (act == '1')
		{
			print_list(stack, space);
			int pc = is_palindrom(stack);
			if (pc > 0)
				printf("String is a palindrom.\n");
			else if (pc == 0)
				printf("String is not a palindrom.\n");
		}
		else if (act == '2')
		{
			char ch;
			printf("Enter symbol: ");
			fflush(stdin);
			scanf("%c", &ch);
			rc = push(&stack, ch, &space);
			if (rc != OK)
				errmsg(rc);
		}
		else if (act == '3')
		{
			char ch;
			printf("Enter string: ");
			fflush(stdin);
			do
			{
				scanf("%c", &ch);
				if (ch != '\n')
				{
					rc = push(&stack, ch, &space);
					if (rc != OK)
						errmsg(rc);
				}
			}
			while (ch != '\n');
		}
		else if (act == '4')
		{
			rc = pop(&stack, &space);
			if (rc != OK)
				errmsg(rc);
		}
		else if (act == '5')
		{
			print_arr(arr);
			int pc = is_palindrom_arr(arr);
			if (pc > 0)
				printf("String is a palindrom.\n");
			else if (pc == 0)
				printf("String is not a palindrom.\n");
		}
		else if (act == '6')
		{
			char ch;
			printf("Enter symbol: ");
			fflush(stdin);
			scanf("%c", &ch);
			rc = push_arr(&arr, ch);
			if (rc != OK)
				errmsg(rc);
		}
		else if (act == '7')
		{
			char ch;
			printf("Enter string: ");
			fflush(stdin);
			do
			{
				scanf("%c", &ch);
				if (ch != '\n')
				{
					rc = push_arr(&arr, ch);
					if (rc != OK)
						errmsg(rc);
				}
			}
			while (ch != '\n');
		}
		else if (act == '8')
			pop_arr(&arr);
		else
			act = 0;
	}
	while (act);
	free_spaces(&space);
	return 0;
}