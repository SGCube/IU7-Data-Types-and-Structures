#include <stdio.h>
#include <string.h>
#include "error.h"
#include "file.h"

ARR_DLL char* ARR_DECL fsearch(FILE *f, const char *word)
{
	if (!f || !word)
		return NULL;
	char *cword = NULL;
	int sc = 1;
	while (sc != EOF && sc == 1 && strcmp(cword, word) != 0)
	{
		cword = NULL;
		sc = fscanf(f, "%s", cword);
	}
	return cword;
}