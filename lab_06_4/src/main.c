#include <stdio.h>
#include <windows.h>

#include "error.h"
#include "struct.h"

/// прототипы функций из библиотек ******************************************

typedef int (__cdecl *fn_fsearch_t)(FILE *, int);

typedef tree_t* (__cdecl *fn_create_node_t)(int);
typedef void (__cdecl *fn_free_tree_t)(tree_t *);
typedef tree_t* (__cdecl *fn_tree_add_t)(tree_t *, tree_t *);
typedef tree_t* (__cdecl *fn_tree_read_t)(FILE *, int *);
typedef tree_t* (__cdecl *fn_tree_search_t)(tree_t *, int);
typedef tree_t* (__cdecl *fn_tree_remove_t)(tree_t *, int);
typedef void (__cdecl *fn_print_node_t)(tree_t *);
typedef void (__cdecl *fn_tree_print_t)(tree_t *, int);
typedef int (__cdecl *fn_height_t)(tree_t *);
typedef tree_t* (__cdecl *fn_balance_t)(tree_t *);
typedef tree_t* (__cdecl *fn_balance_all_t)(tree_t *);
typedef tree_t* (__cdecl *fn_avl_remove_t)(tree_t *, int);

typedef void (__cdecl *fn_export_t)(FILE *, const char *, tree_t*);

typedef int (__cdecl *fn_hread_t)(FILE *, hash_t *, int *);
typedef int (__cdecl *fn_hsearch_t)(int, hash_t *, int);
typedef int (__cdecl *fn_hremove_t)(int, hash_t *, int);
typedef void (__cdecl *fn_hprint_t)(hash_t *, int);

///**************************************************************************

int main(int argc, char **argv)
{
	int rc = OK;	//код ошибки
	
	///*** объявления библиотек *********************************************
	
	HMODULE filelib;
	fn_fsearch_t fsearch;
	
	HMODULE treelib;
	fn_create_node_t create_node;
	fn_free_tree_t free_tree;
	fn_tree_add_t tree_add;
	fn_tree_read_t tree_read;
	fn_tree_search_t tree_search;
	fn_tree_remove_t tree_remove;
	fn_tree_print_t print_tree;
	fn_print_node_t print_node;
	fn_height_t tree_height;
	fn_balance_t balance;
	fn_balance_all_t tree_balance;
	fn_avl_remove_t avl_remove;
	
	fn_export_t tree_export;
	
	HMODULE hashlib;
	fn_hread_t hasht_read;
	fn_hsearch_t hasht_search;
	fn_hremove_t hasht_remove;
	fn_hprint_t hasht_print;
	
	///*** file.dll *********************************************************
	
	filelib = LoadLibrary("lib\\file.dll");
    if (!filelib)
    {
        fprintf(stderr, "Can not open file.dll.\n");
        return ERR_LIB;
    }
	
	fsearch = (fn_fsearch_t) GetProcAddress(filelib, "fsearch");
	
	if (!fsearch)
	{
        printf("Can not load functions (file.dll).\n");
		FreeLibrary(filelib);
        return ERR_LIB;
    }
	
	///*** btree.dll ********************************************************
	
	treelib = LoadLibrary("lib\\btree.dll");
    if (!treelib)
    {
        fprintf(stderr, "Can not open btree.dll.\n");
		FreeLibrary(filelib);
        return ERR_LIB;
    }
	create_node = (fn_create_node_t) GetProcAddress(treelib, "create_node");
	free_tree = (fn_free_tree_t) GetProcAddress(treelib, "free_all");
	tree_add = (fn_tree_add_t) GetProcAddress(treelib, "add");
	tree_read = (fn_tree_read_t) GetProcAddress(treelib, "read");
	tree_search = (fn_tree_search_t) GetProcAddress(treelib, "search");
	tree_remove = (fn_tree_remove_t) GetProcAddress(treelib, "tree_remove");
	print_node = (fn_print_node_t) GetProcAddress(treelib, "print_node");
	print_tree = (fn_tree_print_t) GetProcAddress(treelib, "print");
	tree_height = (fn_height_t) GetProcAddress(treelib, "height");
	balance = (fn_balance_t) GetProcAddress(treelib, "balance");
	tree_balance = (fn_balance_all_t) GetProcAddress(treelib, "balance_all");
	avl_remove = (fn_avl_remove_t) GetProcAddress(treelib, "avl_remove");
	tree_export = (fn_export_t) GetProcAddress(treelib, "export_to_dot");
	
	if (!create_node || !tree_add || !tree_search || !tree_remove ||
		!print_tree || !print_node || !tree_height || !tree_balance ||
		!balance || !avl_remove || !free_tree)
	{
        printf("Can not load functions (btree.dll).\n");
		FreeLibrary(filelib);
		FreeLibrary(treelib);
        return ERR_LIB;
    }
	
	///*** hasht.dll ********************************************************
	
	hashlib = LoadLibrary("lib\\hasht.dll");
	if (!hashlib)
	{
		fprintf(stderr, "Can not open hasht.dll.\n");
		FreeLibrary(filelib);
		FreeLibrary(treelib);
        return ERR_LIB;
	}
	
	hasht_read = (fn_hread_t) GetProcAddress(hashlib, "hread");
	hasht_search = (fn_hsearch_t) GetProcAddress(hashlib, "hsearch");
	hasht_remove = (fn_hremove_t) GetProcAddress(hashlib, "hremove");
	hasht_print = (fn_hprint_t) GetProcAddress(hashlib, "hprint");
	
	if (!hasht_read || !hasht_search || !hasht_remove || !hasht_print)
	{
		printf("Can not load functions (hasht.dll).\n");
		FreeLibrary(filelib);
		FreeLibrary(treelib);
		FreeLibrary(hashlib);
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
	hash_t ht[MAX_SIZE];
	int n = 23;
	
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
			"5 - delete from avl tree\n"
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
		rewind(f);
		if (action == '1')
		{
			int rc = OK;
			tree = tree_read(f, &rc);
			if (rc != OK)
				action = 0;
			if (rc == ERR_EMPTY)
				fprintf(stderr, "File is empty!\n");
			if (rc == ERR_NUMB)
				fprintf(stderr, "Invalid input!\n");
			if (rc == ERR_MEMORY)
				fprintf(stderr, "Memory allocation error!\n");
		}
		else if (action == '2')
			tree = tree_balance(tree);
		else if ((action >= '3' && action <= '5') || (action == '8') ||
			(action == '9'))
		{
			int x;
			printf("Enter a number to search: ");
			
			if (scanf("%d", &x) != 1)
			{
				fprintf(stderr, "Number input error.\n");
				action = 0;
				rc = ERR_NUMB;
			}
			else if (action == '3')
			{
				tree_t *node = tree_search(tree, x);
				if (!node)
					printf("\nNumber was not found.\n");
				else
				{
					printf("\nNumber is found!\nNumber:\t");
					print_node(node);
					printf("Address:\t%p\n", (void *)node);
				}
			}
			else if (action == '4')
				tree = tree_remove(tree, x);
			else if (action == '5')
				tree = avl_remove(tree, x);
			else if (action == '8')
			{
				int ind = hasht_search(x, ht, n);
				if (ind == -1)
					printf("\nNumber was not found.\n");
				else
				{
					printf("\nNumber is found!\n");
					printf("Index:\t%d\n", ind);
				}
			}
			else if (action == '9')
				hasht_remove(x, ht, n);
		}
		else if (action == '6')
		{
			printf("\n");
			print_tree(tree, 0);
			FILE *dot = fopen("tree.gv", "w");
			if (!dot)
				fprintf(stderr, "Can't make dot file!\n");
			else
			{
				tree_export(dot, "tree", tree);
				fclose(dot);
			}
			system("dot.exe -Tpng tree.gv -o tree.png");
			system(".\\tree.png");
		}
		else if (action == '7')
		{
			rc = hasht_read(f, ht, &n);
			if (rc != OK)
				action = 0;
			if (rc == ERR_MEMORY)
				fprintf(stderr, "Hash table cannot be rehashed!\n");
			else if (rc == ERR_NUMB)
				fprintf(stderr, "Invalid input!\n");
			else if (rc == ERR_EMPTY)
				fprintf(stderr, "File is empty!\n");
		}
		else if (action == '0')
		{
			printf("\n");
			hasht_print(ht, n);
		}
		else
			action = 0;
		
		if (action == '3' || action == '6' || action == '0')
		{
			printf("\nEnter any key to continue");
			fflush(stdin);
			getchar();
		}
		
	}
	while (action);
	
	free_tree(tree);
	fclose(f);
	FreeLibrary(filelib);
	FreeLibrary(treelib);
	FreeLibrary(hashlib);
	return rc;
}