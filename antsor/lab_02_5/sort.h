#include "theatre.h"

#ifndef __SORT_H__
#define __SORT_H__

void bsort_key(struct keytable *keys, int len);
void qsort_key(struct keytable *pb, struct keytable *pe);
void bsort_rep(struct spectac *rep, int len);
void qsort_rep(struct spectac *pb, struct spectac *pe);

#endif