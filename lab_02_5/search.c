#include <stdio.h>
#include <string.h>
#include "search.h"

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