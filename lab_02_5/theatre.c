#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "theatre.h"

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

void errmsg(int rc)
{
	switch (rc)
	{
		case ERR_ALLOC:
			fprintf(stderr, "Memory allocation error!\n");
			break;
		case ERR_INPUT:
			fprintf(stderr, "Read error!\n");
			break;
		case ERR_FILE:
			fprintf(stderr, "Couldn't open a file!\n");
			break;
		case ERR_EMPTY:
			fprintf(stderr, "File is empty!\n");
			break;
		case ERR_TYPE:
			fprintf(stderr, "Wrong type of spectacle!\n");
			break;
		case ERR_AGE:
			fprintf(stderr, "Wrong children age for spectacle!\n");
			break;
		case ERR_LONG_STR:
			fprintf(stderr, "Too long string in input!\n");
			break;
		case NOT_ENOUGH:
			fprintf(stderr, "Not enough data!\n");
			break;
		default:
			fprintf(stderr, "Unknown error!\n");
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
		return ERR_INPUT;
	printf("Enter maximal ticket price: ");
	if (scanf("%d", &(temp.max_tprice)) != 1)
		return ERR_INPUT;
	if (temp.max_tprice < 0)
		return ERR_INPUT;
	
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
				return ERR_INPUT;
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

void repert_print(FILE *f, struct spectac *rep, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (f == stdout)
			fprintf(f, "%3d|", (i + 1));
		fprintf(f, "%s|%s|%s|%d|%d|", rep[i].theatre, rep[i].title,
			rep[i].director, rep[i].min_tprice, rep[i].max_tprice);
		if (rep[i].type == CHILD)
			fprintf(f, "C|");
		else
			fprintf(f, "A|");
		if (rep[i].type == ADULT)
		{
			if (rep[i].u_spec.adult == PLAY)
				fprintf(f, "P");
			else if (rep[i].u_spec.adult == DRAMA)
				fprintf(f, "D");
			else if (rep[i].u_spec.adult == COMEDY)
				fprintf(f, "C");
		}
		else
		{
			fprintf(f, "%d|", rep[i].u_spec.child.age);
			if (rep[i].u_spec.child.type == FTALE)
				fprintf(f, "F");
			else if (rep[i].u_spec.child.type == CPLAY)
				fprintf(f, "P");
			else if (rep[i].u_spec.child.type == MUSIC)
			{
				fprintf(f, "M|%s|%s|%d|%d",
					rep[i].u_spec.child.music.composer,
					rep[i].u_spec.child.music.country,
					rep[i].u_spec.child.music.min_age,
					rep[i].u_spec.child.music.duration);
			}
		}
		fprintf(f, "\n");
	}
}