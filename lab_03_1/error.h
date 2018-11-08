#ifndef __ERROR_H__
#define __ERROR_H__

#define OK 0
#define ERR_INPUT -1
#define ERR_FILE -2
#define ERR_EMPTY -3
#define ERR_ALLOC -4
#define ERR_SIZE -5
#define ERR_AMOUNT -6
#define ERR_MATRIX -7
#define ERR_INDEX -8
#define ERR_ZERO -9
#define ERR_MISSED_DATA -10
#define ERR_DIFF_SIZE -11

#define LARGE_SIZE 100

void errmsg(int rc);

#endif