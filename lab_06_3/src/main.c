#include <stdio.h>
#include <windows.h>

#include "error.h"

typedef size_t (__cdecl *fn_getline_t)(char **, size_t *, FILE *f);
typedef char* (__cdecl *fn_fsearch_t)(FILE *f, char*, int *);

int main(int argc, char **argv)
{
	HMODULE filelib;
	fn_getline_t getline;
	fn_fsearch_t fsearch;
	
	filelib = LoadLibrary("lib\\file.dll");
    if (!filelib)
    {
        fprintf(stderr, "Can not open a library.\n");
        return ERR_LIB;
    }
	
	fsearch = (fn_fsearch_t) GetProcAddress(filelib, "fsearch");
	getline = (fn_getline_t) GetProcAddress(filelib, "getline");
	if (!fsearch || !getline)
	{
        printf("Can not load functions.\n");
		FreeLibrary(filelib);
        return ERR_LIB;
    }
	
	if (argc < 2)
	{
		fprintf(stderr, "No input file.\n");
		FreeLibrary(filelib);
		return ERR_CMD;
	}
		
	FILE *f = fopen(argv[1], "r");
	if (!f)
	{
		fprintf(stderr, "Can not open a file.\n");
		FreeLibrary(filelib);
		return ERR_FILE;
	}
	
	char *word = NULL;
	size_t n = 0;
	
	setbuf(stdout, NULL);
	fflush(stdin);
	printf("Enter a word to search: ");
	
	if (getline(&word, &n, stdin) == -1)
	{
		fprintf(stderr, "Word input error.\n");
		FreeLibrary(filelib);
		if (word)
			free(word);
		return ERR_WORD;
	}
	
	printf("Word: %s\n", word);
	
	int rc = OK;
	char *cword = fsearch(f, word, &rc);
	if (rc == ERR_EMPTY)
	{
		fprintf(stderr, "File is empty.\n");
		FreeLibrary(filelib);
		free(word);
		return ERR_WORD;
	}
	if (rc == OK)
	{
		if (!cword)
			printf("Word was not found in file!\n");
		else
			printf("Word is found!\n");
	}
	
	fclose(f);
	FreeLibrary(filelib);
	return 0;
}