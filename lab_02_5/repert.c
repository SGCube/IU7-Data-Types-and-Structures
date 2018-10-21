#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "theatre.h"
#include "sort.h"
#include "fileio.h"

#define OK 0

#define ERR_ALLOC -1
#define ERR_INPUT -2
#define ERR_CHOICE -3
#define ERR_FILE - 4
#define ERR_EMPTY -5
#define ERR_TYPE -6
#define ERR_AGE -7
#define ERR_LONG_STR -8
#define NOT_ENOUGH -9
#define ERR_INDEX -10
#define ERR_VALUE -11

void errmsg(int rc)
{
	switch (rc)
	{
		case ERR_ALLOC:
			fprintf(stdout, "Memory allocation error!\n");
			break;
		case ERR_INPUT:
			fprintf(stdout, "Read error!\n");
			break;
		case ERR_FILE:
			fprintf(stdout, "Couldn't open a file!\n");
			break;
		case ERR_EMPTY:
			fprintf(stdout, "File is empty!\n");
			break;
		case ERR_VALUE:
			fprintf(stdout, "Invalid value!\n");
			break;
		case ERR_TYPE:
			fprintf(stdout, "Wrong type of spectacle!\n");
			break;
		case ERR_AGE:
			fprintf(stdout, "Wrong children age for spectacle!\n");
			break;
		case ERR_LONG_STR:
			fprintf(stdout, "Too long string in input!\n");
			break;
		case NOT_ENOUGH:
			fprintf(stdout, "Not enough data!\n");
			break;
		default:
			fprintf(stdout, "Unknown error!\n");
	}
}

int repert_alloc(struct spectac **rep, int len)
{
	if (len % RECORD_N == 0)
	{
		void *t = realloc(*rep, (len + RECORD_N) * sizeof(struct spectac));
		if (!t)
			return ERR_ALLOC;
		else
			*rep = (struct spectac *) t;
	}
	return OK;
}

struct keytable *create_keytable(struct spectac *rep, int len)
{
	struct keytable *keys;
	keys = malloc(len * sizeof(struct keytable));
	if (!keys)
		return NULL;
	for (int i = 0; i < len; i++)
	{
		keys[i].index = i;
		keys[i].val = rep[i].max_tprice;
	}
	return keys;
}

int add_new_record(struct spectac **rep, int *len)
{
	assert(*rep);
	assert(*len >= 0);
	char ch;
	struct spectac temp;
	
	fflush(stdin);
	printf("Enter name of theatre: ");
	if (fgets(temp.theatre, MAX_SLEN + 1, stdin) == NULL)
		return ERR_INPUT;
	strtok(temp.theatre, "\n");
	printf("Enter title: ");
	if (fgets(temp.title, MAX_SLEN + 1, stdin) == NULL)
		return ERR_INPUT;
	strtok(temp.title, "\n");
	printf("Enter director's name: ");
	if (fgets(temp.director, MAX_SLEN + 1, stdin) == NULL)
		return ERR_INPUT;
	strtok(temp.director, "\n");
	printf("Enter minimal ticket price: ");
	if (scanf("%d", &(temp.min_tprice)) != 1)
		return ERR_INPUT;
	if (temp.min_tprice < 0)
		return ERR_VALUE;
	printf("Enter maximal ticket price: ");
	if (scanf("%d", &(temp.max_tprice)) != 1)
		return ERR_INPUT;
	if (temp.max_tprice < temp.min_tprice)
		return ERR_VALUE;
	
	fflush(stdin);
	printf("Choose spectale's age category:\n"
		"0 - for children; 1 - for adults\n");
	if (scanf("%c", &ch) != 1)
		return ERR_INPUT;
	if (ch == '0')
	{
		temp.type = CHILD;
		printf("Enter recommended age: ");
		if (scanf("%d", &(temp.u_spec.child.age)) != 1)
			return ERR_INPUT;
		if (temp.u_spec.child.age < 0 || temp.u_spec.child.age > 17)
			return ERR_AGE;
		printf("Choose spectale's genre:\n"
			"0 - fairy tale; 1 - play; 2 - musical\n");
		
		fflush(stdin);
		if (scanf("%c", &ch) != 1)
			return ERR_INPUT;
		if (ch == '0')
			temp.u_spec.child.type = FTALE;
		else if (ch == '1')
			temp.u_spec.child.type = CPLAY;
		else if (ch == '2')
		{
			fflush(stdin);
			temp.u_spec.child.type = MUSIC;
			printf("Enter composer's name: ");
			if (fgets(temp.u_spec.child.music.composer,
				MAX_SLEN + 1, stdin) == NULL)
				return ERR_INPUT;
			strtok(temp.u_spec.child.music.composer, "\n");
			printf("Enter country: ");
			if (fgets(temp.u_spec.child.music.country,
				MIN_SLEN + 1, stdin) == NULL)
				return ERR_INPUT;
			strtok(temp.u_spec.child.music.country, "\n");
			printf("Enter minimal age: ");
			if (scanf("%d", &(temp.u_spec.child.music.min_age)) != 1)
				return ERR_INPUT;
			if (temp.u_spec.child.music.min_age < temp.u_spec.child.age ||
				temp.u_spec.child.music.min_age >= 18)
				return ERR_AGE;
			printf("Enter duration (in minutes): ");
			if (scanf("%d", &(temp.u_spec.child.music.duration)) != 1)
				return ERR_INPUT;
			if (temp.u_spec.child.music.duration < 0)
				return ERR_VALUE;
		}
		else
			return ERR_TYPE;
	}
	else if (ch == '1')
	{
		temp.type = ADULT;
		fflush(stdin);
		printf("Choose spectale's genre:\n"
			"0 - play; 1 - drama; 2 - comedy\n");
		if (scanf("%c", &ch) != 1)
			return ERR_INPUT;
		if (ch == '0')
			temp.u_spec.adult = PLAY;
		else if (ch == '1')
			temp.u_spec.adult = DRAMA;
		else if (ch == '2')
			temp.u_spec.adult = COMEDY;
		else
			return ERR_TYPE;
	}
	else
		return ERR_TYPE;
	
	if (repert_alloc(rep, *len) == ERR_ALLOC)
		return ERR_ALLOC;
	(*rep)[*len] = temp;
	*len += 1;
	return OK;
}

void delete_record(struct spectac *rep, int *len, int ind)
{
	for (int i = ind - 1; i < *len; i++)
		rep[i] = rep[i+1];
	*len -= 1;
}

int search_record(struct spectac *rep, int len)
{
	int age, duration;
	printf("Enter age: ");
	if (scanf("%d", &age) != 1 || age < 0 || age > 17)
		return ERR_AGE;
	printf("Enter duration (in minutes): ");
	if (scanf("%d", &duration) != 1 || duration < 0)
		return ERR_VALUE;
	
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].type == CHILD && rep[i].u_spec.child.type == MUSIC &&
			rep[i].u_spec.child.age <= age &&
			rep[i].u_spec.child.musical < dur)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

void sort_record(struct spectac *rep, int len)
{
	fprintf(stdout, "Choose method:\n"
				"1 - Bubble sort of whole repertoire\n"
				"2 - Bubble sort of keytable\n"
				"3 - Quick sort of whole repertoire\n"
				"4 - Quick sort of keytable\n");
	if (scanf("%d", &i) || i < 1 || i > 4)
	{
		fprintf(stdout, "Invalid index for method!\n");
		return;
	}
	if (i == 1 || i == 3)
	{
		if (i == 1)
			bsort_rep(rep, len);
		else
			qsort_rep(rep, rep + len);
		FILE *f = fopen("repert.txt", "w");
		if (!f)
			fprintf(stdout, "Couldn't open a file for writing!\n");
		else
		{
			repert_print_by(f, rep, len);
			fclose(f);
		}
	}
	if (i == 2 || i == 4)
	{
		struct keytable *keys = create_keytable(rep, len);
		if (!keys)
		{
			fprintf(stdout, "Keytable memory allocation error!\n");
			return;
		}
		if (i == 2)
			bsort_key(keys, len);
		else
			qsort_key(keys, keys + len);
		FILE *f = fopen("repert.txt", "w");
		if (!f)
			fprintf(stdout, "Couldn't open a file for writing!\n");
		else
		{
			repert_print_by_table(f, rep, keys, len);
			fclose(f);
		}
		free(keys);
	}
}

