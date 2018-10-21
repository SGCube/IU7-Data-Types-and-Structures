#ifndef __THEATRE_H__
#define __THEATRE_H__

#define RECORD_N 40
#define MAX_SLEN 20
#define MIN_SLEN 15
#define PRICE_DIGITS 8

#define OK 0

#define ERR_ALLOC -1
#define ERR_INPUT -2
#define ERR_CHOICE -3
#define ERR_FILE - 4
#define ERR_EMPTY -5
#define ERR_TYPE -6
#define ERR_AGE -7
#define ERR_LONG_STR -8
#define NOT_ENOUGH -9
#define ERR_INDEX -10
#define ERR_VALUE -11

typedef enum {CHILD, ADULT} spect_type;
typedef enum {FTALE, CPLAY, MUSIC} child_t;
typedef enum {PLAY, DRAMA, COMEDY} adult_t;

struct spectac
{
	char theatre[MAX_SLEN + 1];
	char title[MAX_SLEN + 1];
	char director[MAX_SLEN + 1];
	int min_tprice;
	int max_tprice;
	spect_type type;
	union
	{	
		struct
		{
			child_t type;
			int age;
			struct
			{
				char composer[MAX_SLEN];
				char country[MIN_SLEN];
				int min_age;
				int duration;
			} music;
		} child;
		adult_t adult;
	} u_spec;
};

struct keytable
{
	int index;
	int val;
};

#endif