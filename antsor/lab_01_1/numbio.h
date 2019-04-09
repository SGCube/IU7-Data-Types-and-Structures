#ifndef __NUMBIO_H__
#define __NUMBIO_H__

#define ERR_DIGIT -1
#define ERR_EMPTY -2
#define ERR_FORMAT -3

int read_real(int mantis[], int expon[]);

int read_int(int numb[]);

int str_to_int(char str[], int slen, int digits[], int max_size);

int str_to_real(char str[], int slen, int mant[], int expon[]);

void print_numb(int mantis[], int expon[]);

#endif