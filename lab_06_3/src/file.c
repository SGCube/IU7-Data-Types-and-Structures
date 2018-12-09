#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "file.h"

#define BUF_SIZE 20

ARR_DLL size_t ARR_DECL getword(char **lineptr, size_t *n, FILE *stream)
{
	if (stream == NULL || lineptr == NULL || n == NULL)
		return -1;
	if (*n == 0 || *lineptr == NULL)
	{
		*lineptr = malloc(BUF_SIZE * sizeof(char));
		*n = BUF_SIZE;
	}
	
	size_t len = 0;
	char ch = '\n';

	while (fscanf(stream, "%c", &ch) == 1 && ch != '\n' && ch != ' ')
	{
		if (len + 1 >= *n)
		{
			char *t = (char *)realloc(*lineptr,
				(*n + BUF_SIZE) * sizeof(char));
			if (!t)
				return -1;
			*lineptr = t;
			*n += BUF_SIZE;
		}
		*(*lineptr + len++) = ch;
	}
	
	if (len == 0)
		return -1;
	*(lineptr + len) = '\0';
	return len;
}

ARR_DLL char* ARR_DECL fsearch(FILE *f, const char *word, int *rc)
{
	if (!f)
	{
		*rc = ERR_FILE;
		return NULL;
	}
	if (!word)
	{
		*rc = ERR_WORD;
		return NULL;
	}
	
	*rc = OK;
	char ch;
	if (fscanf(f, "%c", &ch) == EOF)
	{
		*rc = ERR_EMPTY;
		return NULL;
	}
	
	char *cword = NULL;
	short int found = 0;
	size_t n = 0;
	while (getword(&cword, &n, f) != -1 && !found)
	{
		if (strcmp(cword, word) == 0)
			found = 1;
	}
	if (found)
		return cword;
	if (cword)
		free(cword);
	return NULL;
}