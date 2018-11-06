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
	FILE *fa = stdin, *fb = stdin;
	
	if (file_open(argc, argv, &f1, &f2, &fout) == ERR_FILE)
	{
		printf("File open error!\n");
		return ERR_FILE;
	}
	
	if (argc == 2)
		fa = f1;
	if (argc == 3)
		fb = f2;
	
	rc = matrix_input(a, stdin);
	if (rc != OK)
	{
		printf("Error a!\n");
		free_matrix(a);
		return rc;
	}
	print_matrix(a, stdout);
	rc = matrix_input(b, stdin);
	if (rc != OK)
	{
		printf("Error b!\n");
		free_matrix(a);
		free_matrix(b);
		return rc;
	}
	print_matrix(b, stdout);
		
	return rc;
}