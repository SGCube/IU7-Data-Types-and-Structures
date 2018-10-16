#ifndef __THEATRE_H__
#define __THEATRE_H__

#define RECORD_N 40
#define MAX_SLEN 50
#define MIN_SLEN 25

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
			int age;
			child_t type;
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

struct key_int
{
	int index;
	int value;
};

void errmsg(int rc);
int repert_alloc(struct spectac **rep, int len);
int add_new_record(struct spectac **rep, int *len);
void delete_record(struct spectac *rep, int *len, int ind);
void repert_print(FILE *f, struct spectac *rep, int len);

#endif