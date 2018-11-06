#ifndef __MATRIXIO_H__
#define __MATRIXIO_H__

#include "matrix.h"

void free_matrix(matrix ma);
void print_matrix(matrix ma, FILE *f);
void print_matrix_std(matrix ma, FILE *f);
int read_matrix(matrix *ma, FILE *f);

#endif