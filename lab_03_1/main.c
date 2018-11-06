#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "error.h"

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	int rc = OK;
	matrix a = { 0, 0, 0, NULL, NULL, NULL };
	matrix b = { 0, 0, 0, NULL, NULL, NULL };
	matrix c = { 0, 0, 0, NULL, NULL, NULL };
	
	FILE *f1 = NULL, *f2 = NULL, *fout = NULL;
	
	if (file_open(argc, argv, &f1, &f2, &fout) == ERR_FILE)
	{
		printf("File open error!\n");
		return ERR_FILE;
	}
	
	if (argc == 1)
	{
		matrix_input(a);
	}
		
	return rc;
}