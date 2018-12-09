#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "file.h"

#define BUF_SIZE 20

ARR_DLL size_t ARR_DECL getline(char **lineptr, size_t *n, FILE *stream)
{
	if (stream == NULL || lineptr == NULL || n == NULL)
		return -1;
	if (*n == 0 || *lineptr == NULL)
	{
		*lineptr = malloc(BUF_SIZE * sizeof(char));
		*n = BUF_SIZE;
	}
	
	size_t len = 0;
	char buff[BUF_SIZE];
	char *ptr = *lineptr;
	char *t;
	size_t buff_len = 0;

	do
	{
		if (fgets(buff, BUF_SIZE, stream) == NULL)
			return -1;
		buff_len = strlen(buff);
		if (len + buff_len + 1 > *n)
		{
			t = (char *)realloc(*lineptr,
				(len + BUF_SIZE) * sizeof(char));
			if (!t)
				return -1;
			*lineptr = t;
			*n = len + BUF_SIZE;
		}
		ptr = *lineptr + len;
		len += buff_len;
		for (const char *pc = buff; *pc != '\0'; pc++, ptr++)
			*ptr = *pc;
	}
	while (!feof(stream) && buff[buff_len - 1] != '\n');
	
	if (len == 0)
		return -1;
	(*lineptr)[len - 1] = '\0';
	*n = len + 1;
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
	rewind(f);
	
	char *cword = NULL;
	short int found = 0;
	size_t n = 0;
	
	while (getline(&cword, &n, f) != -1 && !found)
		if (strcmp(cword, word) == 0)
			found = 1;
		
	if (found)
		return cword;
	if (cword)
		free(cword);
	return NULL;
}