#include <stdio.h>
#include <stdlib.h>
#include "theatre.h"

void repert_print(struct spectac *rep, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%s|%s|%s|%d|%d|", rep[i].theatre, rep[i].name, rep[i].director,
			rep[i].min_tprice, rep[i].max_tprice);
		if (rep[i].type == CHILD)
			printf("For children|");
		else
			printf("For adults|");
		if (rep[i].type == ADULT)
		{
			if (rep[i].u_spec.adult == PLAY)
				printf("Play");
			else if (rep[i].u_spec.adult == DRAMA)
				printf("Drama");
			else if (rep[i].u_spec.adult == COMEDY)
				printf("Comedy");
		}
		else
		{
			printf("%d|", rep[i].u_spec.child.age);
			if (rep[i].u_spec.child.type == FTALE)
				printf("Fairy Tale");
			else if (rep[i].u_spec.child.type == CPLAY)
				printf("Play");
			else if (rep[i].u_spec.child.type == MUSIC)
			{
				printf("Musical|%s|%s|%d|%d",
					rep[i].u_spec.child.music.composer,
					rep[i].u_spec.child.music.country,
					rep[i].u_spec.child.music.min_age,
					rep[i].u_spec.child.music.duration);
			}
		}
		printf("\n");
	}
}