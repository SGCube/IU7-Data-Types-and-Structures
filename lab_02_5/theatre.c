#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "theatre.h"
#include "search.h"

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

int add_new_record(struct spectac **rep, int *len)
{
	assert(*rep);
	assert(*len >= 0);
	char ch;
	struct spectac temp;
	/*char theatre[MAX_SLEN + 1];
	char title[MAX_SLEN + 1];
	char director[MAX_SLEN + 1];
	char composer[MAX_SLEN + 1];
	char country[MIN_SLEN + 1];
	int min_tprice, max_tprice;
	spect_type type;
	child_t type;
	adult_t adult;
	int age;
	int min_age, duration;*/
	
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
		return ERR_TYPE;
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
			return ERR_TYPE;
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
			if (temp.u_spec.child.music.min_age < 0 ||
				temp.u_spec.child.music.min_age > temp.u_spec.child.age)
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
			return ERR_TYPE;
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

int search_record(struct spectac *rep, int len, int attr)
{
	char str[MAX_SLEN + 1];
	char ch;
	int i, k = -1;
	fflush(stdin);
	switch (attr)
	{
		case 1:
			printf("Enter name of theatre: ");
			if (fgets(str, MAX_SLEN + 1, stdin) == NULL)
				return ERR_INPUT;
			strtok(str, "\n");
			k = search_by_theatre(rep, len, str);
			break;
		case 2:
			printf("Enter title: ");
			if (fgets(str, MAX_SLEN + 1, stdin) == NULL)
				return ERR_INPUT;
			strtok(str, "\n");
			k = search_by_title(rep, len, str);
			break;
		case 3:
			printf("Enter director's name: ");
			if (fgets(str, MAX_SLEN + 1, stdin) == NULL)
				return ERR_INPUT;
			strtok(str, "\n");
			k = search_by_director(rep, len, str);
			break;
		case 4:
			printf("Enter minimal ticket price: ");
			if (scanf("%d", &i) != 1)
				return ERR_INPUT;
			if (i < 0)
				return ERR_VALUE;
			k = search_by_minprice(rep, len, i);
			break;
		case 5:
			printf("Enter maximal ticket price: ");
			if (scanf("%d", &i) != 1)
				return ERR_INPUT;
			if (i < 0)
				return ERR_VALUE;
			k = search_by_maxprice(rep, len, i);
			break;
		case 6:
			printf("Choose spectale's age category:\n"
			"0 - for children; 1 - for adults\n");
			if (scanf("%c", &ch) != 1)
				return ERR_INPUT;
			if (ch != '0' && ch != '1')
				return ERR_TYPE;
			if (ch == '0')
				k = search_by_type(rep, len, CHILD);
			if (ch == '1')
				k = search_by_type(rep, len, ADULT);
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		default:
			fprintf(stdout, "Invalid type of attribute!\n");
	}
	if (k == -1)
		return ERR_INPUT;
	if (k == 0)
		fprintf(stdout, "There is no such records!\n");
	return OK;
	/*{
		printf("Enter recommended age: ");
		if (scanf("%d", &(temp.u_spec.child.age)) != 1)
			return ERR_INPUT;
		if (temp.u_spec.child.age < 0 || temp.u_spec.child.age > 17)
			return ERR_AGE;
	}
	{
		printf("Choose spectale's genre:\n"
			"0 - fairy tale; 1 - play; 2 - musical\n");
		if (scanf("%c", &ch) != 1)
			return ERR_TYPE;
	}
	{
		temp.u_spec.child.type = MUSIC;
		printf("Enter composer's name: ");
		if (fgets(temp.u_spec.child.music.composer,
			MAX_SLEN + 1, stdin) == NULL)
			return ERR_INPUT;
		strtok(temp.u_spec.child.music.composer, "\n");
	}
	{
		printf("Enter country: ");
		if (fgets(temp.u_spec.child.music.country,
			MIN_SLEN + 1, stdin) == NULL)
			return ERR_INPUT;
		strtok(temp.u_spec.child.music.country, "\n");
	}
	{
		printf("Enter minimal age: ");
		if (scanf("%d", &(temp.u_spec.child.music.min_age)) != 1)
			return ERR_INPUT;
		if (temp.u_spec.child.music.min_age < 0 ||
			temp.u_spec.child.music.min_age > temp.u_spec.child.age)
			return ERR_AGE;
	}
	{
		printf("Enter duration (in minutes): ");
		if (scanf("%d", &(temp.u_spec.child.music.duration)) != 1)
			return ERR_INPUT;
		if (temp.u_spec.child.music.duration < 0)
			return ERR_INPUT;
	}
	{
		printf("Choose spectale's genre:\n"
			"0 - play; 1 - drama; 2 - comedy\n");
		if (scanf("%c", &ch) != 1)
			return ERR_TYPE;
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
	}*/
}

void record_print(FILE *f, struct spectac rep)
{
	fprintf(f, "%s|%s|%s|%d|%d|", rep.theatre, rep.title,
		rep.director, rep.min_tprice, rep.max_tprice);
	if (rep.type == CHILD)
		fprintf(f, "C|");
	else
		fprintf(f, "A|");
	if (rep.type == ADULT)
	{
		if (rep.u_spec.adult == PLAY)
			fprintf(f, "P");
		else if (rep.u_spec.adult == DRAMA)
			fprintf(f, "D");
		else if (rep.u_spec.adult == COMEDY)
			fprintf(f, "C");
	}
	else
	{
		fprintf(f, "%d|", rep.u_spec.child.age);
		if (rep.u_spec.child.type == FTALE)
			fprintf(f, "F");
		else if (rep.u_spec.child.type == CPLAY)
			fprintf(f, "P");
		else if (rep.u_spec.child.type == MUSIC)
		{
			fprintf(f, "M|%s|%s|%d|%d",
				rep.u_spec.child.music.composer,
				rep.u_spec.child.music.country,
				rep.u_spec.child.music.min_age,
				rep.u_spec.child.music.duration);
		}
	}
	fprintf(f, "\n");
}

void repert_print(FILE *f, struct spectac *rep, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (f == stdout)
			fprintf(f, "%3d|", (i + 1));
		record_print(f, rep[i]);
	}
	if (len == 0 && f == stdout)
		fprintf(f, "There is no records!\n");
}