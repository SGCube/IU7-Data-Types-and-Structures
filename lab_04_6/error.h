#ifndef __ERROR_H__
#define __ERROR_H__

#define OK 0
#define ERR_INPUT -1
#define ERR_ALLOC -2
#define ERR_OVERFLOW -3

void errmsg(int rc);

#endif