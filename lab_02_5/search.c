#include <stdio.h>
#include <string.h>
#include "search.h"

#define ERR_VALUE -11

int search_by_theatre(struct spectac *rep, int len, char *str)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (strcmp(rep[i].theatre, str) == 0)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k;
}

int search_by_title(struct spectac *rep, int len, char *str)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (strcmp(rep[i].title, str) == 0)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k;
}

int search_by_director(struct spectac *rep, int len, char *str)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (strcmp(rep[i].director, str) == 0)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k;
}

int search_by_minprice(struct spectac *rep, int len, int val)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].min_tprice == val)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

int search_by_maxprice(struct spectac *rep, int len, int val)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].max_tprice == val)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k;
}

int search_by_type(struct spectac *rep, int len, spect_type t)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].type == t)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

int search_by_ctype(struct spectac *rep, int len, child_t t)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].type == CHILD && rep[i].u_spec.child.type == t)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

int search_by_atype(struct spectac *rep, int len, adult_t t)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].type == ADULT && rep[i].u_spec.adult == t)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

int search_by_age(struct spectac *rep, int len, int val)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].type == CHILD && rep[i].u_spec.child.age >= val)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

int search_by_composer(struct spectac *rep, int len, char *str)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].type == CHILD && rep[i].u_spec.child.type == MUSIC && 
			strcmp(rep[i].u_spec.child.music.composer, str) == 0)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

int search_by_country(struct spectac *rep, int len, char *str)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].type == CHILD && rep[i].u_spec.child.type == MUSIC && 
			strcmp(rep[i].u_spec.child.music.country, str) == 0)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

int search_by_minage(struct spectac *rep, int len, int val)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].type == CHILD && rep[i].u_spec.child.type == MUSIC && 
			rep[i].u_spec.child.music.min_age > val)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

int search_by_duration(struct spectac *rep, int len, int val)
{
	int k = 0;
	for (int i = 0; i < len; i++)
		if (rep[i].type == CHILD && rep[i].u_spec.child.type == MUSIC && 
			rep[i].u_spec.child.music.duration == val)
		{
			k++;
			fprintf(stdout, "%3d|%3d|", k, (i + 1));
			record_print(stdout, rep[i]);
		}
	return k; 
}

void search_record(struct spectac *rep, int len)
{
	char str[MAX_SLEN + 1];
	char ch;
	int i, k = -1;
	fflush(stdin);
	fprintf(stdout, "Choose attribute:\n"
				"1 - theatre\n"
				"2 - title\n"
				"3 - director\n"
				"4 - minimal ticket price\n"
				"5 - maximal ticket price\n"
				"6 - age category\n"
				"7 - spectacle type (child)\n"
				"8 - spectacle type (adult)\n"
				"9 - recommended age (child)\n"
				"10 - composer (musical)\n"
				"11 - country (musical)\n"
				"12 - minimal allowed age (musical)\n"
				"13 - duration (musical)\n");
	if (scanf("%d", &i) != 1 || i < 1 || i > 13)
	{
		fprintf(stdout, "Invalid type of attribute!\n");
		return;
	}
	
	switch (attr)
	{
		case 1:
			printf("Enter name of theatre: ");
			if (fgets(str, MAX_SLEN + 1, stdin) == NULL)
				break;
			strtok(str, "\n");
			k = search_by_theatre(rep, len, str);
			break;
		case 2:
			printf("Enter title: ");
			if (fgets(str, MAX_SLEN + 1, stdin) == NULL)
				break;
			strtok(str, "\n");
			k = search_by_title(rep, len, str);
			break;
		case 3:
			printf("Enter director's name: ");
			if (fgets(str, MAX_SLEN + 1, stdin) == NULL)
				break;
			strtok(str, "\n");
			k = search_by_director(rep, len, str);
			break;
		case 4:
			printf("Enter minimal ticket price: ");
			if (scanf("%d", &i) != 1 || i < 0)
				break;
			k = search_by_minprice(rep, len, i);
			break;
		case 5:
			printf("Enter maximal ticket price: ");
			if (scanf("%d", &i) != 1 || i < 0)
				break;
			k = search_by_maxprice(rep, len, i);
			break;
		case 6:
			printf("Choose spectale's age category:\n"
			"0 - for children; 1 - for adults\n");
			if (scanf("%c", &ch) != 1)
				break;
			if (ch != '0' && ch != '1')
				break;
			if (ch == '0')
				k = search_by_type(rep, len, CHILD);
			if (ch == '1')
				k = search_by_type(rep, len, ADULT);
		case 7:
			printf("Choose spectale's genre:\n"
				"0 - fairy tale; 1 - play; 2 - musical\n");
			if (scanf("%c", &ch) != 1)
				break;
			if (ch != '0' && ch != '1' && ch != '2')
				break;
			if (ch == '0')
				k = search_by_ctype(rep, len, FTALE);
			if (ch == '1')
				k = search_by_ctype(rep, len, CPLAY);
			if (ch == '2')
				k = search_by_ctype(rep, len, MUSIC);
			break;
		case 8:
			printf("Choose spectale's genre:\n"
				"0 - play; 1 - drama; 2 - comedy\n");
			if (scanf("%c", &ch) != 1)
				break;
			if (ch != '0' && ch != '1' && ch != '2')
				break;
			if (ch == '0')
				k = search_by_atype(rep, len, PLAY);
			if (ch == '1')
				k = search_by_atype(rep, len, DRAMA);
			if (ch == '2')
				k = search_by_atype(rep, len, COMEDY);
			break;
		case 9:
			printf("Enter recommended age: ");
			if (scanf("%d", &i) != 1 || i < 0 || i > 17)
				break;
			k = search_by_age(rep, len, i);
			break;
		case 10:
			printf("Enter composer's name: ");
			if (fgets(str, MAX_SLEN + 1, stdin) == NULL)
				break;
			strtok(str, "\n");
			k = search_by_composer(rep, len, str);
			break;
		case 11:
			printf("Enter country: ");
			if (fgets(str, MAX_SLEN + 1, stdin) == NULL)
				break;
			strtok(str, "\n");
			k = search_by_country(rep, len, str);
			break;
		case 12:
			printf("Enter minimal age: ");
			if (scanf("%d", &i) != 1 || i < 0 || i > 17)
				break;
			if (i < 0 || i > 17)
				break;
			k = search_by_minage(rep, len, i);
			break;
		case 13:
			printf("Enter duration (in minutes): ");
			if (scanf("%d", &i) != 1 || i < 0)
				break;
			k = search_by_duration(rep, len, i);
			break;
		default:
			fprintf(stdout, "Invalid type of attribute!\n");
	}
	if (k == -1)
		errmsg(ERR_VALUE);
	if (k == 0)
		fprintf(stdout, "There is no such records!\n");
}