#include "theatre.h"

#ifndef __SEARCH_H__
#define __SEARCH_H__

int search_by_theatre(struct spectac *rep, int len, char *str);
int search_by_title(struct spectac *rep, int len, char *str);
int search_by_director(struct spectac *rep, int len, char *str);
int search_by_minprice(struct spectac *rep, int len, int value);
int search_by_maxprice(struct spectac *rep, int len, int value);
int search_by_type(struct spectac *rep, int len, spect_type t);
/*int search_by_age(struct spectac *rep, int len, int value);
int search_by_ctype(struct spectac *rep, int len, child_t t);
int search_by_atype(struct spectac *rep, int len, adult_t t);
int search_by_composer(struct spectac *rep, int len, char *str);
int search_by_country(struct spectac *rep, int len, char *str);
int search_by_minage(struct spectac *rep, int len, int value);
int search_by_duration(struct spectac *rep, int len, int value);*/

#endif