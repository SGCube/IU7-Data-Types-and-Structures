#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

int read_repert(struct spectac **rep, int *rep_len, FILE *f)
{
	assert(rep);
	assert(f);
	int sc, len = 0, slen = 0;
	short int cur = 0;
	char ch = '|';
	struct spectac *spc = (*rep);
	char *strptr = spc->theatre;
	
	sc = fscanf(f, "%c", &ch);
	if (sc == EOF)
		return ERR_EMPTY;
	else if (sc != 1 || ch == '|' || ch == '\n')
		return ERR_INPUT;
	rewind(f);
	
	while (sc != EOF)
	{
		if (sc != EOF)
		{
			sc = fscanf(f, "%c", &ch);
			if (sc != 1 && sc != EOF)
				return ERR_INPUT;
		}
		if (ch == '\n' || sc == EOF)
		{
			if (cur != 7)
			{
				if (cur == 0 && slen == 0)
					sc = EOF;
				else
					return NOT_ENOUGH;
			}
			else
			{
				len++;
				*rep_len = len;
				slen = 0;
				cur = 0;
				if (repert_alloc(rep, len) == ERR_ALLOC)
					return ERR_ALLOC;
				spc = &((*rep)[len]);
				strptr = spc->theatre;
			}
		}
		else if (ch == '|')
		{
			if (cur > 6)
				return ERR_INPUT;
			slen = 0;
			if (cur == 0 || cur == 1 || cur == 4 || cur == 5)
				*strptr = '\0';
			if (cur == 0)
				strptr = spc->title;
			else if (cur == 1)
				strptr = spc->director;
			else if (cur == 2)
			{
				sc = fscanf(f, "%d|%d|%c", &(spc->min_tprice),
					&(spc->max_tprice), &ch);
				if (sc == EOF || sc != 3)
					return ERR_INPUT;
				if (ch == 'C')
					spc->type = CHILD;
				else if (ch == 'A')
					spc->type = ADULT;
				else
					return ERR_TYPE;
			}
			else if (spc->type == ADULT)
			{
				if (cur == 3)
				{
					sc = fscanf(f, "%c", &ch);
					if (sc == EOF || sc != 1)
						return ERR_INPUT;
					if (ch == 'P')
						spc->u_spec.adult = PLAY;
					else if (ch == 'D')
						spc->u_spec.adult = DRAMA;
					else if (ch == 'C')
						spc->u_spec.adult = COMEDY;
					else
						return ERR_TYPE;
					cur = 6;
				}
				else
					return ERR_INPUT;
			}
			else if (cur == 3)
			{
				sc = fscanf(f, "%d|%c", &(spc->u_spec.child.age), &ch);
				if (sc == EOF || sc != 2)
					return ERR_INPUT;
				if (spc->u_spec.child.age < 0 ||
					spc->u_spec.child.age > 17)
					return ERR_AGE;
				if (ch == 'F')
					spc->u_spec.child.type = FTALE;
				else if (ch == 'P')
					spc->u_spec.child.type = CPLAY;
				else if (ch == 'M')
					spc->u_spec.child.type = MUSIC;
				else
					return ERR_TYPE;
				if (spc->u_spec.child.type != MUSIC)
					cur = 6;
			}
			else if (spc->u_spec.child.type == MUSIC && cur == 4)
				strptr = spc->u_spec.child.music.composer;
			else if (spc->u_spec.child.type == MUSIC && cur == 5)
				strptr = spc->u_spec.child.music.country;
			else if (spc->u_spec.child.type == MUSIC && cur == 6)
			{
				sc = fscanf(f, "%d|%d",
					&(spc->u_spec.child.music.min_age),
					&(spc->u_spec.child.music.duration));
				if (sc == EOF || sc != 2)
					return ERR_INPUT;
			}
			cur++;
		}
		else if ((cur == 6 && slen >= MIN_SLEN) || slen >= MAX_SLEN)
			return ERR_LONG_STR;
		else
		{
			if (cur > 6)
				return ERR_INPUT;
			*(strptr++) = ch;
			slen++;
		}
	}
	return OK;
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

void repert_print_by_table(FILE *f, struct spectac *rep,
	struct keytable *keys, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (f == stdout)
			fprintf(f, "%3d|", (i + 1));
		record_print(f, rep[keys[i].index]);
	}
	if (len == 0 && f == stdout)
		fprintf(f, "There is no records!\n");
}

