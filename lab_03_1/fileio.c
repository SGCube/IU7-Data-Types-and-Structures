#include <stdio.h>
#include "fileio.h"
#include "error.h"

int file_open(int argc, char **argv, FILE **f1, FILE **f2, FILE **fout)
{
	if (argc == 2)
	{
		*f1 = fopen(argv[1], "r");
		if (!*f1)
			return ERR_FILE;
	}
	if (argc == 3)
	{
		*f2 = fopen(argv[2], "r");
		if (!*f2)
		{
			fclose(*f1);
			return ERR_FILE;
		}
	}
	if (argc == 4)
	{
		*fout = fopen(argv[3], "w");
		if (!*fout)
		{
			fclose(*f1);
			fclose(*f2);
			return ERR_FILE;
		}
	}
	return OK;
}

void file_close(FILE *f1, FILE *f2, FILE *fout)
{
	if (f1)
		fclose(f1);
	if (f2)
		fclose(f2);
	if (fout)
		fclose(fout);
}