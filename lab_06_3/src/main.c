#include <stdio.h>
#include <windows.h>

#include "error.h"
#include "struct.h"

/// прототипы функций из библиотек ******************************************

typedef size_t (__cdecl *fn_getline_t)(char **, size_t *, FILE *f);
typedef char* (__cdecl *fn_fsearch_t)(FILE *f, char*, int *);

typedef tree_t* (__cdecl *fn_create_node_t)(char *);
typedef tree_t* (__cdecl *fn_tree_add_t)(tree_t *, tree_t *);
typedef tree_t* (__cdecl *fn_tree_search_t)(tree_t *, char *);
typedef void (__cdecl *fn_print_node_t)(tree_t *);
typedef void (__cdecl *fn_tree_print_t)(tree_t *, int);

///**************************************************************************

int main(int argc, char **argv)
{
	int rc = OK;	//код ошибки
	
	///*** объявления библиотек *********************************************
	
	HMODULE filelib;
	fn_getline_t getline;
	fn_fsearch_t fsearch;
	
	HMODULE treelib;
	fn_create_node_t create_node;
	fn_tree_add_t tree_add;
	fn_tree_search_t tree_search;
	fn_tree_print_t tree_print;
	fn_print_node_t print_node;
	
	///*** file.dll *********************************************************
	
	filelib = LoadLibrary("lib\\file.dll");
    if (!filelib)
    {
        fprintf(stderr, "Can not open file.dll.\n");
        return ERR_LIB;
    }
	
	fsearch = (fn_fsearch_t) GetProcAddress(filelib, "fsearch");
	getline = (fn_getline_t) GetProcAddress(filelib, "getline");
	
	if (!fsearch || !getline)
	{
        printf("Can not load functions (file.dll).\n");
		FreeLibrary(filelib);
        return ERR_LIB;
    }
	
	///*** btree.dll ********************************************************
	
	treelib = LoadLibrary("lib\\btree.dll");
    if (!filelib)
    {
        fprintf(stderr, "Can not open btree.dll.\n");
		FreeLibrary(filelib);
        return ERR_LIB;
    }
	create_node = (fn_create_node_t) GetProcAddress(treelib, "create_node");
	tree_add = (fn_tree_add_t) GetProcAddress(treelib, "add");
	tree_search = (fn_tree_search_t) GetProcAddress(treelib, "search");
	print_node = (fn_print_node_t) GetProcAddress(treelib, "print_node");
	tree_print = (fn_tree_print_t) GetProcAddress(treelib, "print");
	
	if (!create_node || !tree_add || !tree_search || !tree_print ||
		!print_node)
	{
        printf("Can not load functions (btree.dll).\n");
		FreeLibrary(filelib);
		FreeLibrary(treelib);
        return ERR_LIB;
    }
	
	///*** проверка входных аргументов **************************************
	
	if (argc < 2)
	{
		fprintf(stderr, "No input file.\n");
		FreeLibrary(filelib);
		FreeLibrary(treelib);
		return ERR_CMD;
	}
		
	FILE *f = fopen(argv[1], "r");
	if (!f)
	{
		fprintf(stderr, "Can not open a file.\n");
		FreeLibrary(filelib);
		FreeLibrary(treelib);
		return ERR_FILE;
	}
	
	///*** объявление структур **********************************************
	
	tree_t *tree = NULL;
	
	///*** запуск основного меню ********************************************
	
	char action = 0;
	do
	{
		setbuf(stdout, NULL);
		fflush(stdin);
		printf("\n"
			"BINARY SEARCH TREE ACTIONS:\n"
			"===========================\n"
			"1 - make tree from file\n"
			"2 - balance tree\n"
			"3 - search in tree\n"
			"4 - delete from tree\n"
			"5 - delete from tree & balance it\n"
			"6 - print tree\n"
			"===========================\n"
			"HASH TABLE ACTIONS:\n"
			"7 - make hash table from file\n"
			"8 - search in hash table\n"
			"9 - delete from hash table\n"
			"0 - print hash table\n"
			"===========================\n"
			"Enter action: ");
		scanf("%c", &action);
		if (action == '1')
		{
			char *word = NULL;
			size_t n = 0;
			while (getline(&word, &n, f) != -1)
			{
				tree_t *node = create_node(word);
				if (!node)
				{
					fprintf(stderr, "Memory allocation error!\n");
					if (word)
						free(word);
					action = 0;
					rc = ERR_MEMORY;
				}
				else
					tree = tree_add(tree, node);
				word = NULL;
				n = 0;
			}
		}
		else if (action == '3')
		{
			char *word = NULL;
			size_t n = 0;
			
			fflush(stdin);
			printf("Enter a word to search: ");
			
			if (getline(&word, &n, stdin) == -1)
			{
				fprintf(stderr, "Word input error.\n");
				if (word)
					free(word);
				action = 0;
				rc = ERR_WORD;
			}
			else
			{
				tree_t *node = tree_search(tree, word);
				if (!node)
					printf("Word was not found!\n");
				else
				{
					printf("Word is found!\nWord: ");
					print_node(node);
					printf("Address: %p", (void *)node);
				}
			}
		}
		else
			action = 0;
	}
	while (action);
	
	/*char *word = NULL;
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
	}*/
	
	//printf("Word: %s\n", word);
	
	/*int rc = OK;
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
	}*/
	
	fclose(f);
	FreeLibrary(filelib);
	FreeLibrary(treelib);
	return rc;
}