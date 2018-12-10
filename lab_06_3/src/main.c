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
typedef tree_t* (__cdecl *fn_del_node_t)(tree_t **, char *);
typedef void (__cdecl *fn_print_node_t)(tree_t *);
typedef void (__cdecl *fn_tree_print_t)(tree_t *, int);
typedef int (__cdecl *fn_height_t)(tree_t *);
typedef tree_t* (__cdecl *fn_balance_t)(tree_t *);

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
	fn_del_node_t del_node;
	fn_tree_print_t print_tree;
	fn_print_node_t print_node;
	fn_height_t tree_height;
	fn_balance_t tree_balance;
	
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
	del_node = (fn_del_node_t) GetProcAddress(treelib, "del_node");
	print_node = (fn_print_node_t) GetProcAddress(treelib, "print_node");
	print_tree = (fn_tree_print_t) GetProcAddress(treelib, "print");
	tree_height = (fn_height_t) GetProcAddress(treelib, "height");
	tree_balance = (fn_balance_t) GetProcAddress(treelib, "balance");
	
	if (!create_node || !tree_add || !tree_search || !del_node ||
		!print_tree || !print_node || !tree_height || !tree_balance)
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
		printf("\n");
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
		else if (action == '2')
			tree = tree_balance(tree);
		else if (action >= '3' && action <= '4')
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
				tree_t *node = NULL;
				if (action == '3')
					node = tree_search(tree, word);
				else if (action == '4')
					node = del_node(&tree, word);
				if (!node)
					printf("\nWord was not found.\n");
				else
				{
					printf("\nWord is found!\nWord:\t");
					print_node(node);
					printf("Address:\t%p\n", (void *)node);
				}
			}
		}
		else if (action == '6')
		{
			printf("\n");
			print_tree(tree, 0);
		}
		else
			action = 0;
		
		if (action > '1')
		{
			printf("\nEnter any key to continue");
			fflush(stdin);
			getchar();
		}
		
	}
	while (action);
	
	fclose(f);
	FreeLibrary(filelib);
	FreeLibrary(treelib);
	return rc;
}