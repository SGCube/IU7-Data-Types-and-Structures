#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include "error.h"
#include "file.h"
#include "btree.h"
#include "avltree.h"
#include "hasht.h"

typedef unsigned long int tick_t;

tick_t tick(void)
{
	tick_t d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}

void file_random(int n, FILE *f)
{
	if (!f || n <= 0)
		return;
	
	int *arr = (int *) malloc(n * sizeof(int));
	if (!arr)
		return;
	
	int rmin = 1, rmax = n;
	int rwidth = rmax - rmin + 1;
	
	int *arr2 = (int *) malloc(rwidth * sizeof(int));
	if (!arr2)
		return;
	 
	for (int i = 0; i < rwidth; i++)
		arr2[i] = rmin + i;
	 
	int n2 = rwidth;
	for (int i = 0; i < n; i++)
	{
		int index = rand() % n2;
		arr[i] = arr2[index];
		n2--;
		arr2[index] = arr2[n2];
	}
	
	for (int i = 0; i < n; i++)
		fprintf(f, "%d ", arr[i]);
	
	free(arr);
	free(arr2);
}

int main()
{
	srand(time(NULL));
	
	///*** запуск тестирования **********************************************
					
	int sizes[] = { 25, 50, 100, 250, 500 };
	
	tick_t trmv_tree[5], tsrc_tree[5], trmv_avl[5], tsrc_avl[5],
		trmv_hash[5], tsrc_hash[5], trmv_file[5], tsrc_file[5];
		
	int krmv_tree[5], ksrc_tree[5], krmv_avl[5], ksrc_avl[5],
		krmv_hash[5], ksrc_hash[5], krmv_file[5], ksrc_file[5];
	
	unsigned int file_size[5];
	
	hash_t ht[MAX_SIZE];
	int n = 13;
	
	for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++)
	{
		tick_t start = 0, end = 0;
		
		///***** BTREE **********************
		trmv_tree[i] = 0;
		tsrc_tree[i] = 0;
		krmv_tree[i] = 0;
		ksrc_tree[i] = 0;
		
		for (int j = 0, key = 1; j < sizes[i]; j++, key++)
		{
			int kcmp = 0;
			
			tree_t *tree = NULL;
			
			FILE *f = fopen("test.txt", "w");
			assert(f);
			file_random(sizes[i], f);
			fclose(f);
			f = fopen("test.txt", "r");
			assert(f);
			
			int rc = OK;
			tree = tree_read(f, &rc);
			assert(rc == OK);
			fclose(f);
			
			start = tick();
			tree_search(tree, key, &kcmp);
			end = tick();
			tsrc_tree[i] += end - start;
			ksrc_tree[i] += kcmp;
			
			kcmp = 0;
			
			start = tick();
			tree = tree_remove(tree, key, &kcmp);
			end = tick();
			trmv_tree[i] += end - start;
			krmv_tree[i] += kcmp;
			
			tree_free(tree);
		}
		tsrc_tree[i] /= sizes[i];
		trmv_tree[i] /= sizes[i];
		ksrc_tree[i] /= sizes[i];
		krmv_tree[i] /= sizes[i];
		
		///***** AVL **********************
		trmv_avl[i] = 0;
		tsrc_avl[i] = 0;
		krmv_avl[i] = 0;
		ksrc_avl[i] = 0;
		
		for (int j = 0, key = 1; j < sizes[i]; j++, key++)
		{
			int kcmp = 0;
			
			tree_t *tree = NULL;
			
			FILE *f = fopen("test.txt", "w");
			assert(f);
			file_random(sizes[i], f);
			fclose(f);
			f = fopen("test.txt", "r");
			assert(f);
			
			int rc = OK;
			tree = tree_read(f, &rc);
			assert(rc == OK);
			
			fclose(f);
			
			tree_balance(tree);
			
			start = tick();
			tree_search(tree, key, &kcmp);
			end = tick();
			tsrc_avl[i] += end - start;
			ksrc_avl[i] += kcmp;
			
			kcmp = 0;
			
			start = tick();
			tree = avl_remove(tree, key, &kcmp);
			end = tick();
			trmv_avl[i] += end - start;
			krmv_avl[i] += kcmp;
			
			tree_free(tree);
		}
		tsrc_avl[i] /= sizes[i];
		trmv_avl[i] /= sizes[i];
		ksrc_avl[i] /= sizes[i];
		krmv_avl[i] /= sizes[i];
		
		///***** HTABLE **********************
		trmv_hash[i] = 0;
		tsrc_hash[i] = 0;
		krmv_hash[i] = 0;
		ksrc_hash[i] = 0;
		
		for (int j = 0, key = 1; j < sizes[i]; j++, key++)
		{
			int kcmp = 0;
			n = 13;
			
			FILE *f = fopen("test.txt", "w");
			assert(f);
			file_random(sizes[i], f);
			fclose(f);
			f = fopen("test.txt", "r");
			assert(f);
			
			int rc = hread(f, ht, &n);
			assert(rc == OK);
			
			fclose(f);
			
			start = tick();
			hsearch(key, ht, n, &kcmp);
			end = tick();
			tsrc_hash[i] += end - start;
			ksrc_hash[i] += kcmp;
			
			kcmp = 0;
			
			start = tick();
			hremove(key, ht, n, &kcmp);
			end = tick();
			trmv_hash[i] += end - start;
			krmv_hash[i] += kcmp;
		}
		tsrc_hash[i] /= sizes[i];
		trmv_hash[i] /= sizes[i];
		ksrc_hash[i] /= sizes[i];
		krmv_hash[i] /= sizes[i];
		
		///***** FILE **********************
		tsrc_file[i] = 0;
		trmv_file[i] = 0;
		ksrc_file[i] = 0;
		krmv_file[i] = 0;
		
		for (int j = 0, key = 1; j < sizes[i]; j++, key++)
		{
			int kcmp = 0;
			
			FILE *f = fopen("test.txt", "w");
			assert(f);
			file_random(sizes[i], f);
			fclose(f);
			
			f = fopen("test.txt", "r");
			assert(f);
			start = tick();
			fsearch(f, n, &kcmp);
			end = tick();
			tsrc_file[i] += end - start;
			ksrc_file[i] += kcmp;
			fseek(f, 0, SEEK_END);
			file_size[i] = ftell(f);
			fclose(f);
			
			kcmp = 0;
			
			start = tick();
			fremove("test.txt", n, &kcmp);
			end = tick();
			krmv_file[i] += kcmp;
			trmv_file[i] += end - start;
		}
		tsrc_file[i] /= sizes[i];
		trmv_file[i] /= sizes[i];
		ksrc_file[i] /= sizes[i];
		krmv_file[i] /= sizes[i];
	}
	
	printf("\t\t\t\tsearch\n\n");
	printf("%3s\t%12s\t%12s\t%12s\t%12s\n", "N", "bintree", "avltree",
		"hashtable", "file");
		
	for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++)
	{
		printf("%3d\t%9.5lf ms\t%9.5lf ms\t%9.5lf ms\t%9.4lf ms\n", sizes[i],
			(double) tsrc_tree[i] / CLOCKS_PER_SEC,
			(double) tsrc_avl[i] / CLOCKS_PER_SEC,
			(double) tsrc_hash[i] / CLOCKS_PER_SEC,
			(double) tsrc_file[i] / CLOCKS_PER_SEC);
		printf("\t%8d cmp\t%8d cmp\t%8d cmp\t%8d cmp\n",
			ksrc_tree[i], ksrc_avl[i], ksrc_hash[i], ksrc_file[i]);
		printf("\t%6u bytes\t%6u bytes\t%6u bytes\t%6u bytes\n",
			(unsigned) (sizes[i] * sizeof(tree_t)),
			(unsigned) (sizes[i] * sizeof(tree_t)),
			(unsigned) (sizes[i] * sizeof(hash_t)),
			file_size[i]);
	}
		
	
	printf("\n\t\t\t\tremove\n\n");
	printf("%3s\t%12s\t%12s\t%12s\t%12s\n", "N", "bintree", "avltree",
		"hashtable", "file");
		
	for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++)
	{
		printf("%3d\t%9.5lf ms\t%9.5lf ms\t%9.5lf ms\t%9.4lf ms\n", sizes[i],
			(double) trmv_tree[i] / CLOCKS_PER_SEC,
			(double) trmv_avl[i] / CLOCKS_PER_SEC,
			(double) trmv_hash[i] / CLOCKS_PER_SEC,
			(double) trmv_file[i] / CLOCKS_PER_SEC);
		printf("\t%8d cmp\t%8d cmp\t%8d cmp\t%8d cmp\n",
			krmv_tree[i], krmv_avl[i], krmv_hash[i], krmv_file[i]);
		printf("\t%6u bytes\t%6u bytes\t%6u bytes\t%6u bytes\n",
			(unsigned) (sizes[i] * sizeof(tree_t)),
			(unsigned) (sizes[i] * sizeof(tree_t)),
			(unsigned) (sizes[i] * sizeof(hash_t)),
			file_size[i]);
	}
		
	printf("\n");
	system("rm.exe test.txt");
	return 0;
}