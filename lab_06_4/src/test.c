#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include "error.h"
#include "struct.h"

typedef unsigned long int tick_t;

tick_t tick(void)
{
	tick_t d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}

/// прототипы функций из библиотек ******************************************

typedef int (__cdecl *fn_fsearch_t)(FILE *f, int);

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

typedef int (__cdecl *fn_hread_t)(FILE *, hash_t *, int *);
typedef int (__cdecl *fn_hsearch_t)(int, hash_t *, int);
typedef int (__cdecl *fn_hremove_t)(int, hash_t *, int);
typedef void (__cdecl *fn_hprint_t)(hash_t *, int);

int main()
{
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
	
	///*** запуск тестирования **********************************************
	
	char *str[] = { "test_time/in_10.txt", 
					"test_time/in_25.txt",
					"test_time/in_50.txt" };
					
	int sizes[] = { 10, 25, 50};
	int keys[] = { 3, 8, 34 };
	
	tick_t trmv_tree[5], tsrc_tree[5], trmv_avl[5], tsrc_avl[5],
		trmv_hash[5], tsrc_hash[5];
	
	hash_t ht[MAX_SIZE];
	int n = 13;
	
	for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++)
	{
		tick_t start = 0, end = 0;
		trmv_tree[i] = 0;
		tsrc_tree[i] = 0;
		
		for (int j = 0; j < 1000; j++)
		{
			tree_t *tree = NULL;
			FILE *f = fopen(str[i], "r");
			assert(f);
			
			int rc = OK;
			tree = tree_read(f, &rc);
			assert(rc == OK);
			
			fclose(f);
			
			start = tick();
			tree_search(tree, keys[i]);
			end = tick();
			tsrc_tree[i] += end - start;
			
			start = tick();
			tree = tree_remove(tree, keys[i]);
			end = tick();
			trmv_tree[i] += end - start;
			
			free_tree(tree);
		}
		tsrc_tree[i] /= 1000;
		trmv_tree[i] /= 1000;
		
		trmv_avl[i] = 0;
		tsrc_avl[i] = 0;
		
		for (int j = 0; j < 1000; j++)
		{
			tree_t *tree = NULL;
			FILE *f = fopen(str[i], "r");
			assert(f);
			
			int rc = OK;
			tree = tree_read(f, &rc);
			assert(rc == OK);
			
			fclose(f);
			
			tree_balance(tree);
			
			start = tick();
			tree_search(tree, keys[i]);
			end = tick();
			tsrc_avl[i] += end - start;
			
			start = tick();
			tree = avl_remove(tree, keys[i]);
			end = tick();
			trmv_avl[i] += end - start;
			
			free_tree(tree);
		}
		tsrc_avl[i] /= 1000;
		trmv_avl[i] /= 1000;
		
		trmv_hash[i] = 0;
		tsrc_hash[i] = 0;
		
		for (int j = 0; j < 1000; j++)
		{
			n = 13;
			FILE *f = fopen(str[i], "r");
			assert(f);
			
			int rc = hasht_read(f, ht, &n);
			assert(rc == OK);
			
			fclose(f);
			
			start = tick();
			hasht_search(keys[i], ht, n);
			end = tick();
			tsrc_hash[i] += end - start;
			
			start = tick();
			hasht_remove(keys[i], ht, n);
			end = tick();
			trmv_hash[i] += end - start;
		}
		tsrc_hash[i] /= 1000;
		trmv_hash[i] /= 1000;
	}
	
	printf("\t\tsearch\n\n");
	printf("%3s\t%7s\t%7s\t%7s\n", "N", "bintree", "avltree", "hashtab");
	for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++)
		printf("%3d\t%7lu\t%7lu\t%7lu\n", sizes[i], tsrc_tree[i],
		tsrc_avl[i], tsrc_hash[i]);
	
	printf("\n\t\tremove\n\n");
	printf("%3s\t%7s\t%7s\t%7s\n", "N", "bintree", "avltree", "hashtab");
	for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++)
		printf("%3d\t%7lu\t%7lu\t%7lu\n", sizes[i], trmv_tree[i],
		trmv_avl[i], trmv_hash[i]);
	printf("\n");
	
	return 0;
}