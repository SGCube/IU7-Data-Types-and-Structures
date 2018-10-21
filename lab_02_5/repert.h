#include "theatre.h"

#ifndef __REPERT_H__
#define __REPERT_H__

void errmsg(int rc);
int repert_alloc(struct spectac **rep, int len);
struct keytable *create_keytable(struct spectac *rep, int len);
int add_new_record(struct spectac **rep, int *len);
void delete_record(struct spectac *rep, int *len, int ind);
int search_record(struct spectac *rep, int len);
void write_repert(struct spectac *rep, int rep_len);

#endif