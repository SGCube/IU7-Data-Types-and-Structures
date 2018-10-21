#include <stdio.h>
#include <stdlib.h>
#include "theatre.h"
#include "fileio.h"
#include "sort.h"
#include "repert.h"

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
#define ERR_INDEX -10
#define ERR_VALUE -11

int main(void)
{
	int rc = OK, rep_len = 0, fc = OK;
	char ch = 0;
	int i = 0;
	short int to_write();
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
			"1 - read repertoire from file\n"
			"2 - view repertoire\n"
			"3 - add new record\n"
			"4 - remove record\n"
			"5 - search for musical kids spectacles by age & duration\n"
			"6 - sort records\n"
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
				if (repert)
					free(repert);
				return ERR_FILE;
			}
			rc = read_repert(&repert, &rep_len, f);
			fclose(f);
			if (rc != OK)
				errmsg(rc);
		}
		else if (ch == '2')
			repert_printf(repert, rep_len);
		else if (ch == '3')
		{
			fc = add_new_record(&repert, &rep_len);
			if (fc != OK)
				errmsg(rc);
			else
				write_repert(repert, rep_len);
		}
		else if (ch >= '4' && ch <= '6' && rep_len == 0)
			fprintf(stdout, "There is no records!\n");
		else if (ch == '4')
		{
			fprintf(stdout, "Enter index of record to remove: ");
			if (fscanf(stdin, "%d", &i) != 1)
				fprintf(stdout, "Index input error!\n");
			else if (i < 1 || i > rep_len)
				fprintf(stdout, "Invalid index!\n");
			else
			{
				delete_record(repert, &rep_len, i);
				write_repert(repert, rep_len);
			}
		}
		else if (ch == '5')
		{
			fc = search_record(repert, rep_len);
			if (fc < 0)
				errmsg(fc);
		}
		else if (ch == '6')
			sort_record(repert, rep_len);
		else
			ch = 0;
	}
	while (rc == OK && ch != 0);
	
	if (repert)
		free(repert);
	return OK;
}

