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
	int stack_n = 0;
	char arr[ARR_SIZE];
	int arr_n = 0;
	setbuf(stdout, NULL);
	do
	{
		setbuf(stdout, NULL);
		fflush(stdin);
		printf("\nMENU:\n"
			"1 - stack state (dynamic list)\n"
			"2 - add symbol to stack (dynamic list)\n"
			"3 - add string to stack (dynamic list)\n"
			"4 - remove symbol from stack (dynamic list)\n"
			"5 - clear stack (dynamic list)\n"
			"6 - stack state (static array)\n"
			"7 - add symbol to stack (static array)\n"
			"8 - add string to stack (static array)\n"
			"9 - remove symbol from stack (static array)\n"
			"0 - clear stack (static list)\n"
			"other - exit\n"
			"Enter action: ");
		scanf("%c", &act);
		printf("\n");
		if (act == '1')
		{
			print_list(stack, &stack_n, space);
			int pc = is_palindrom(&stack, &stack_n);
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
			list *node = create_node(ch, &space);
			if (!node)
				errmsg(ERR_ALLOC);
			else
				push(&stack, &stack_n, node);
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
					list *node = create_node(ch, &space);
					if (!node)
					{
						errmsg(ERR_ALLOC);
						ch = '\n';
					}
					else
						push(&stack, &stack_n, node);
				}
			}
			while (ch != '\n');
		}
		else if (act == '4')
		{
			list *node = pop(&stack, &stack_n);
			if (!node)
				errmsg(rc);
			frspc *tmp = malloc(sizeof(frspc));
			if (!tmp)
				return ERR_ALLOC;
			tmp->next = space;
			space = tmp;
			space->data = (void *)node;
		}
		else if (act == '5')
			stack_free(&stack, &stack_n);
		else if (act == '6')
		{
			print_arr(arr, &arr_n);
			int pc = is_palindrom_arr(arr, &arr_n);
			if (pc > 0)
				printf("String is a palindrom.\n");
			else if (pc == 0)
				printf("String is not a palindrom.\n");
		}
		else if (act == '7')
		{
			char ch;
			printf("Enter symbol: ");
			fflush(stdin);
			scanf("%c", &ch);
			rc = push_arr(arr, &arr_n, ch);
			if (rc != OK)
				errmsg(rc);
		}
		else if (act == '8')
		{
			char ch;
			printf("Enter string: ");
			fflush(stdin);
			do
			{
				scanf("%c", &ch);
				if (ch != '\n')
				{
					rc = push_arr(arr, &arr_n, ch);
					if (rc != OK)
						errmsg(rc);
				}
			}
			while (ch != '\n');
		}
		else if (act == '9')
			printf("%c\n", pop_arr(arr, &arr_n));
		else if (act == '0')
			clear_arr(arr, &arr_n);
		else
			act = 0;
	}
	while (act);
	stack_free(&stack, &stack_n);
	free_spaces(&space);
	clear_arr(arr, &arr_n);
	return 0;
}