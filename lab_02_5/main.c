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
#define ERR_INDEX -10

int main(void)
{
	int rc = OK, rep_len = 0;
	char ch = 0;
	int i = 0;
	struct spectac *repert = NULL;
	//struct key_int *keytable = NULL;
	
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
				if (repert)
					free(repert);
				return ERR_FILE;
			}
			rc = read_repert(&repert, &rep_len, f);
			fclose(f);
			if (rc != OK)
			{
				errmsg(rc);
				if (repert)
					free(repert);
				return ERR_FILE;
			}
			repert_print(stdout, repert, rep_len);
		}
		else if (ch == '2')
		{
			rc = add_new_record(&repert, &rep_len);
			if (rc != OK)
			{
				errmsg(rc);
				if (repert)
					free(repert);
				return rc;
			}
			FILE *f = fopen("repert.txt", "w");
			if (!f)
				fprintf(stderr, "Couldn't open a file for writing!\n");
			else
			{
				repert_print(f, repert, rep_len);
				fclose(f);
			}
		}
		else if (ch == '3' && rep_len == 0)
			fprintf(stdout, "There is no records!\n");
		else if (ch == '3' && rep_len > 0)
		{
			fprintf(stdout, "Enter index of record to remove: ");
			if (fscanf(stdin, "%d", &i) != 1)
				fprintf(stdout, "Index input error!\n");
			else if (i < 1 || i > rep_len)
				fprintf(stdout, "Invalid index!\n");
			else
			{
				delete_record(repert, &rep_len, i);
				FILE *f = fopen("repert.txt", "w");
				if (!f)
					fprintf(stdout, "Couldn't open a file for writing!\n");
				else
				{
					repert_print(f, repert, rep_len);
					fclose(f);
				}
			}
		}
		else
			ch = 0;
	}
	while (rc == OK && ch != 0);
	
	free(repert);
	return OK;
}