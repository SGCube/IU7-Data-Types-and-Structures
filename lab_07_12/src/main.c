#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "matrix.h"
#include "graph.h"
#include "list.h"
#include "graph_list.h"


int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	
	///*** проверка аргументов командой строки ******************************
	if (argc < 2)
	{
		printf("No arguments. Use -h flag to view help.\n");
		return ERR_CMD;
	}
	if (strcmp(argv[1], "-h") == 0)
	{
		printf("app.exe input.txt\n"
				"* input.txt - input file with graph info\n");
		return OK;
	}
	
	///*** объявление структур **********************************************
	
	FILE *f = NULL;				//исходный файл
	//int *adj_matr = NULL;		//матрица смежности графа
	int node_n = 0;				//количество вершин графа
	int node_i = 0;				//заданная вершнина
	int mindist = 0; 			//заданное минимальное расстояние
	int *d = NULL;				//массив кратчайших расстояний
	int *towns = NULL;			//множество городов-решений
	
	node_t **adj_list = NULL;	//список смежностей графа
	
	int rc = OK;				//код ошибки
	
	///*** чтение файла *****************************************************
	
	f = fopen(argv[1], "r");
	if (!f)
	{
		printf("Can't open a file!\n");
		return ERR_FILE;
	}
	
	rc = fscanf(f, "%d", &node_n);
	if (rc == EOF)
	{
		printf("File is empty!\n");
		fclose(f);
		return ERR_EMPTY;
	}
	if (rc != 1 || node_n <= 0)
	{
		printf("Invalid number of nodes!\n");
		fclose(f);
		return ERR_DATA;
	}
	
	/*adj_matr = alloc_matrix(node_n, node_n);
	if (!adj_matr)
	{
		printf("Memory allocation error!\n");
		fclose(f);
		return ERR_MEMORY;
	}*/
	
	adj_list = calloc(node_n, sizeof(node_t *));
	if (!adj_list)
	{
		printf("Memory allocation error!\n");
		fclose(f);
		return ERR_MEMORY;
	}
	
	/*init_graph(adj_matr, node_n);
	rc = read_graph(f, adj_matr, node_n);
	fclose(f);
	if (rc == ERR_DATA)
	{
		printf("Invalid data!\n");
		free_matrix(adj_matr);
		return ERR_DATA;
	}*/
	
	init_lgraph(adj_list, node_n);
	rc = read_lgraph(f, adj_list, node_n);
	fclose(f);
	if (rc == ERR_DATA)
	{
		printf("Invalid data!\n");
		free_all(adj_list, node_n);
		return ERR_DATA;
	}
	
	///*** чтение данных для анализа ****************************************
	
	printf("Choose node for analysis (from 1 to %d): ", node_n);
	rc = scanf("%d", &node_i);
	if (rc != 1 || node_i < 1 || node_i > node_n)
	{
		printf("Invalid data!\n");
		free_all(adj_list, node_n);
		return ERR_DATA;
	}
	
	printf("Choose minimal distance T: ");
	rc = scanf("%d", &mindist);
	if (rc != 1 || mindist < 0)
	{
		printf("Invalid data!\n");
		free_all(adj_list, node_n);
		return ERR_DATA;
	}
	
	///*** поиск кратчайших путей и анализ данных ***************************
	
	d = short_ldist(adj_list, node_n, node_i - 1);
	if (!d)
	{
		printf("Memory allocation error!\n");
		free_all(adj_list, node_n);
		return ERR_MEMORY;
	}
	
	towns = dist_filter(d, node_n, mindist);
	if (!towns)
	{
		printf("Memory allocation error!\n");
		free_all(adj_list, node_n);
		free(d);
		return ERR_MEMORY;
	}
	
	printf("Towns on distance more than T:\n");
	for (int i = 0; i < node_n; i++)
		if (towns[i])
			printf("%d ", i + 1);
	printf("\n");
	
	///*** создание .dot-файла с описанием графа ****************************
	
	FILE *gv = fopen("graph.gv", "w");
	if (!gv)
		printf("Can't make graph file\n");
	else
	{
		list_to_dot(gv, "roads", adj_list, node_n, node_i, towns);
		fclose(gv);
		system("dot.exe -Tpng graph.gv -o graph.png");
		system(".\\graph.png");
	}
	
	///*** освобождение памяти **********************************************
	
	free_all(adj_list, node_n);
	free(d);
	free(towns);
	
	///**********************************************************************
	
	return OK;
}