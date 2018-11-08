#ifndef __ERROR_H__
#define __ERROR_H__

#define OK 0
#define ERR_INPUT -1
#define ERR_FILE -2
#define ERR_EMPTY -3
#define ERR_SIZE -4
#define ERR_MATRIX -5
#define ERR_ALLOC -6
#define ERR_MISSED_DATA -7
#define ERR_AMOUNT -8

#define LARGE_SIZE 100

void errmsg(int rc);

#endif