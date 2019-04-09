#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "file.h"
#include "btree.h"
#include "avltree.h"
#include "hasht.h"

int main(int argc, char **argv)
{
	int rc = OK;	//код ошибки
	
	///*** проверка входных аргументов **************************************
	
	if (argc < 2)
	{
		fprintf(stderr, "No input file.\n");
		return ERR_CMD;
	}
		
	FILE *f = fopen(argv[1], "r");
	if (!f)
	{
		fprintf(stderr, "Can not open a file.\n");
		return ERR_FILE;
	}
	
	char action = 0;
	int sc = fscanf(f, "%c", &action);
	if (sc == EOF)
	{
		fprintf(stderr, "File is empty.\n");
		fclose(f);
		return ERR_EMPTY;
	}
	rewind(f);
	
	///*** объявление структур **********************************************
	
	tree_t *tree = NULL;		// двоичное дерево поиска
	hash_t ht[MAX_SIZE];		// хэш-таблица
	int n = 23;					// текуший размер хэш-таблицы
	int kcmp = 0;				// количество сравнений
	
	///*** запуск основного меню ********************************************
	
	action = 0;
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
				fprintf(stderr, "Number input error.\n");
			else if (action == '3')
			{
				tree_t *node = tree_search(tree, x, &kcmp);
				if (!node)
					printf("\nNumber was not found.\n");
				else
				{
					printf("\nNumber is found!\nNumber:\t");
					printf("%d\n", node->data);
					printf("Address:\t%p\n", (void *)node);
				}
			}
			else if (action == '4')
				tree = tree_remove(tree, x, &kcmp);
			else if (action == '5')
				tree = avl_remove(tree, x, &kcmp);
			else if (action == '8')
			{
				int ind = hsearch(x, ht, n, &kcmp);
				if (ind == -1)
					printf("\nNumber was not found.\n");
				else
				{
					printf("\nNumber is found!\n");
					printf("Index:\t%d\n", ind);
				}
			}
			else if (action == '9')
				hremove(x, ht, n, &kcmp);
		}
		else if (action == '6')
		{
			printf("\n");
			tree_print(tree, 0);
			FILE *dot = fopen("tree.gv", "w");
			if (!dot)
				fprintf(stderr, "Can't make dot file!\n");
			else
			{
				export_to_dot(dot, "tree", tree);
				fclose(dot);
			}
			system("dot.exe -Tpng tree.gv -o tree.png");
			system(".\\tree.png");
		}
		else if (action == '7')
		{
			rc = hread(f, ht, &n);
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
			hprint(ht, n);
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
	
	if (tree)
		tree_free(tree);
	if (f)
		fclose(f);
	return rc;
}