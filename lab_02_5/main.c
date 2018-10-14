#include <stdio.h>
#include <stdlib.h>
#include "theatre.h"
#include "fileio.h"

#define OK 0

#define ERR_ALLOC -1
#define ERR_INPUT -2
#define ERR_CHOICE -3
#define ERR_FILE - 4
#define ERR_EMPTY -5
#define ERR_TYPE -6
#define ERR_AGE -7
#define ERR_LONG_STR -8
#define NOT_ENOUGH -9

int main(void)
{
	int rc = OK, rep_len = 0;
	char ch = 0;
	struct spectac *repert = NULL;
	
	repert = malloc(RECORD_N * sizeof(struct spectac));
	if (!repert)
	{
		fprintf(stderr, "Memory allocation error!\n");
		return ERR_ALLOC;
	}
	
	setbuf(stdout, NULL);
	
	do
	{
		setbuf(stdin, NULL);
		printf("\nChoose action:\n"
			"1 - view repertoire\n"
			"2 - add new record\n"
			"3 - remove record\n"
			"4 - find record\n"
			"5 - sort records\n"
			"other - quit\n"
			"\nEnter action: ");
		if (scanf("%c", &ch) != 1)
		{
			fprintf(stderr, "Choice input error!\n");
			rc = ERR_INPUT;
		}
		else if (ch == '1')
		{
			FILE *f = fopen("repert.txt", "r");
			if (!f)
			{
				fprintf(stderr, "Couldn't open a file!\n");
				return ERR_FILE;
			}
			rc = read_repert(&repert, &rep_len, f);
			fclose(f);
			if (rc == ERR_ALLOC)
				fprintf(stderr, "Memory allocation error!\n");
			else if (rc == ERR_INPUT)
				fprintf(stderr, "Read error!\n");
			else if (rc == ERR_FILE)
				fprintf(stderr, "Couldn't open a file!\n");
			else if (rc == ERR_EMPTY)
				fprintf(stderr, "File is empty!\n");
			else if (rc == ERR_TYPE)
				fprintf(stderr, "Wrong type of spectacle!\n");
			else if (rc == ERR_AGE)
				fprintf(stderr, "Wrong children age for spectacle!\n");
			else if (rc == ERR_LONG_STR)
				fprintf(stderr, "Too long string in input!\n");
			else if (rc == NOT_ENOUGH)
				fprintf(stderr, "Not enough data!\n");
			if (rc != OK)
				return rc;
			printf("\n");
			printf("%d - out!\n", rep_len);
			repert_print(repert, rep_len);
		}
		else
			ch = 0;
	}
	while (rc == OK && ch != 0);
	
	free(repert);
	return OK;
}