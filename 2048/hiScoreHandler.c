/*
 * hiscore.c
 *
 *  Created on: Nov 13, 2014
 *      Author: kobethuy
 */

#include <stdio.h> // fopen(), fclose(), fscanf()
#include <stdlib.h> // malloc()
#include <time.h>

struct Highscore {
	char *name;
	int score;
	char *time;
};

void initHiScore(struct Highscore hs[]);
void getHiScore(struct Highscore hs[]);
void setHiScore(struct Highscore hs[]);
void sortHiScore(struct Highscore hs[]);

void sortHiScore(struct Highscore hs[]) {

	int i, j;
	for (i = 0; i < 9; ++i) {

		for (j = 0; j < 9; ++j) {

			if (hs[j].score < hs[j + 1].score) {
				struct Highscore tmp = hs[j];
				hs[j] = hs[j + 1];
				hs[j + 1] = tmp;
			}
		}
	}
 }

void getHiScore(struct Highscore hs[]) {

	int foo = 0, read = 1;
	FILE *fd;

	if ((fd = fopen("hiscore.txt", "r")) == NULL) {
		fd = fopen("hiscore.txt", "w");
		initHiScore(hs);
		setHiScore(hs);
	}

	while (read != EOF) {
		hs[foo].name = malloc( sizeof(char) * 11);
		hs[foo].time = malloc( sizeof(char) * 17);
		read = fscanf(fd, "%s %d %s", hs[foo].name, &(hs[foo].score), hs[foo].time);
		foo++;
	}

	hs[foo - 1].name = "NONE";
	hs[foo - 1].time = "--:--_--/--/----";

	fclose(fd);
}

void setHiScore(struct Highscore hs[]) {

	FILE *fd;

	fd = fopen("hiscore.txt", "w");
	sortHiScore(hs);

	for (int foo = 0; foo < 10; foo++)
		fprintf(fd, "%s %d %s\n", hs[foo].name, hs[foo].score, hs[foo].time);

	fclose(fd);
}

void initHiScore(struct Highscore hs[]) {

	for (int a = 0; a < 10; a++) {
		hs[a].name = "NONE";
		hs[a].score = 0;
		hs[a].time = "--:--_--/--/----";
	}
}


/*
int main(void) {

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char *timeLocal = malloc(sizeof(char) * 17);

	sprintf(timeLocal, "%d:%d %d/%d/%d", tm.tm_hour, tm.tm_min, tm.tm_mday, tm.tm_mon, tm.tm_year + 1900);

	printf("%s\n", timeLocal);

	return 0;
}
*/



