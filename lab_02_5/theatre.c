#include <stdio.h>
#include <stdlib.h>
#include "theatre.h"

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
	if (rc == ERR_ALLOC)
		fprintf(stderr, "Memory allocation error!\n");
	else if (rc == ERR_INPUT)
		fprintf(stderr, "Read error!\n");
	else if (rc == ERR_FILE)
		fprintf(stderr, "Couldn't open a file!\n");
	else if (rc == ERR_EMPTY)
		fprintf(stderr, "File is empty!\n");
	else if (rc == ERR_TYPE)
		fprintf(stderr, "Wrong type of spectacle!\n");
	else if (rc == ERR_AGE)
		fprintf(stderr, "Wrong children age for spectacle!\n");
	else if (rc == ERR_LONG_STR)
		fprintf(stderr, "Too long string in input!\n");
	else if (rc == NOT_ENOUGH)
		fprintf(stderr, "Not enough data!\n");
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
	
	printf("Enter name of theatre: ");
	if (fgets(temp.theatre, MAX_SLEN + 1, stdin) == NULL)
		return ERR_INPUT;
	printf("Enter title: ");
	if (fgets(temp.title, MAX_SLEN + 1, stdin) == NULL)
		return ERR_INPUT;
	printf("Enter director's name: ");
	if (fgets(temp.director, MAX_SLEN + 1, stdin) == NULL)
		return ERR_INPUT;
	printf("Enter minimal ticket price: ");
	if (scanf("%d", temp->min_tprice) != 1)
		return ERR_INPUT;
	if (temp.min_tprice < 0)
		return ERR_INPUT;
	printf("Enter maximal ticket price: ");
	if (scanf("%d", temp->max_tprice) != 1)
		return ERR_INPUT;
	if (temp.max_tprice < 0)
		return ERR_INPUT;
	
	printf("Choose spectale's age category:\n"
		"0 - for children; 1 - for adults\n");
	if (scanf("%c", &ch) != 1)
		return ERR_TYPE;
	if (ch == '0')
	{
		temp.type = CHILD;
		printf("Enter recommended age: ");
		if (scanf("%d", temp.u_spec.child->age) != 1)
			return ERR_INPUT;
		if (temp.u_spec.child.age < 0 || temp.u_spec.child.age > 17)
			return ERR_AGE;
		printf("Choose spectale's genre:\n"
			"0 - fairy tale; 1 - play; 2 - musical\n");
		if (scanf("%c", &ch) != 1)
			return ERR_TYPE;
		if (ch == '0')
			temp.u_spec.child.type = FTALE;
		else if (ch == '1')
			temp.u_spec.child.type = CPLAY;
		else if (ch == '2')
		{
			temp.u_spec.child.type = MUSIC;
			printf("Enter composer's name: ");
			if (fgets(temp.u_spec.child.music.composer,
				MAX_SLEN + 1, stdin) == NULL)
				return ERR_INPUT;
			printf("Enter country: ");
			if (fgets(temp.u_spec.child.music.country,
				MIN_SLEN + 1, stdin) == NULL)
				return ERR_INPUT;
			printf("Enter minimal age: ");
			if (scanf("%d", temp.u_spec.child.music->min_age) != 1)
				return ERR_INPUT;
			if (temp.u_spec.child.music.min_age < 0 ||
				temp.u_spec.child.music.min_age > temp.u_spec.child.age)
				return ERR_AGE;
			printf("Enter duration (in minutes): ");
			if (scanf("%d", temp.u_spec.child.music->duration) != 1)
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
	
	if (len % RECORD_N == 0)
	{
		void *t = realloc(*rep, (len + RECORD_N) * sizeof(struct spectac));
		if (!t)
			return ERR_ALLOC;
		else
			*rep = (struct spectac *) t;
	}
	(*rep)[*len] = temp;
	*len += 1;
	return OK;
}

void repert_print(struct spectac *rep, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%s|%s|%s|%d|%d|", rep[i].theatre, rep[i].title,
			rep[i].director, rep[i].min_tprice, rep[i].max_tprice);
		if (rep[i].type == CHILD)
			printf("For children|");
		else
			printf("For adults|");
		if (rep[i].type == ADULT)
		{
			if (rep[i].u_spec.adult == PLAY)
				printf("Play");
			else if (rep[i].u_spec.adult == DRAMA)
				printf("Drama");
			else if (rep[i].u_spec.adult == COMEDY)
				printf("Comedy");
		}
		else
		{
			printf("%d|", rep[i].u_spec.child.age);
			if (rep[i].u_spec.child.type == FTALE)
				printf("Fairy Tale");
			else if (rep[i].u_spec.child.type == CPLAY)
				printf("Play");
			else if (rep[i].u_spec.child.type == MUSIC)
			{
				printf("Musical|%s|%s|%d|%d",
					rep[i].u_spec.child.music.composer,
					rep[i].u_spec.child.music.country,
					rep[i].u_spec.child.music.min_age,
					rep[i].u_spec.child.music.duration);
			}
		}
		printf("\n");
	}
}