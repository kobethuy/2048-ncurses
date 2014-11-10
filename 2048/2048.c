/*
 * 	2048.c
 *
 *  Created on: Nov 2, 2014
 *  Author: kobethuy
 */

#include <curses.h>
#include <stdlib.h>
#include <strings.h>

/* Direction. */
typedef enum {
	dUP,
	dDOWN,
	dLEFT,
	dRIGHT
} dtype;

int **board;
int boardSize, width = 21, height = 18, score = 0, hiscore = 0; // "score" to be implemented.

int powerOf2(int a) {
	if (a == 0) {
		return 1;
	} else {
		return 2 * powerOf2(a - 1);
	}
}

int logOf2(int a) {
	int foo = 0;
	while ((powerOf2(foo)) != a)
		foo++;
	return foo;
}

void slide(dtype dir) {

	switch (dir) {

		case dLEFT:
			for (int loop = 0; loop < 4; loop++) {
				for (int foo = 0; foo < 4; foo++) {
					for (int bar = 0; bar < 3; bar++) {
						if (board[foo][bar] != 0) {
							continue;
						} else {
							board[foo][bar] = board[foo][bar + 1];
							board[foo][bar + 1] = 0;
						}
					}
				}
			}
			break;

		case dRIGHT:
			for (int loop = 0; loop < 4; loop++) {
				for (int foo = 0; foo < 4; foo++) {
					for (int bar = 3; bar > 0; bar--) {
						if (board[foo][bar] != 0) {
							continue;
						} else {
							board[foo][bar] = board[foo][bar - 1];
							board[foo][bar - 1] = 0;
						}
					}
				}
			}
			break;

		case dUP:
			for (int loop = 0; loop < 4; loop++) {
				for (int foo = 0; foo < 3; foo++) {
					for (int bar = 0; bar < 4; bar++) {
						if (board[foo][bar] != 0) {
							continue;
						} else {
							board[foo][bar] = board[foo + 1][bar];
							board[foo + 1][bar] = 0;
						}
					}
				}
			}
			break;

		case dDOWN:
			for (int loop = 0; loop < 4; loop++) {
				for (int foo = 3; foo > 0; foo--) {
					for (int bar = 0; bar < 4; bar++) {
						if (board[foo][bar] != 0) {
							continue;
						} else {
							board[foo][bar] = board[foo - 1][bar];
							board[foo - 1][bar] = 0;
						}
					}
				}
			}
			break;

		default:
			break;
	}
}

void join(dtype dir) {

	switch(dir) {

		case dLEFT:
			for (int foo = 0; foo < 4; foo++) {
				for (int bar = 0; bar < 3; bar++) {
					if (board[foo][bar] == board[foo][bar + 1]) {
						board[foo][bar] += board[foo][bar];
						board[foo][bar + 1] = 0;
						score += board[foo][bar];
					}

				}
			}
			break;

		case dRIGHT:
			for (int foo = 0; foo < 4; foo++) {
				for (int bar = 3; bar > 0; bar--) {
					if (board[foo][bar] == board[foo][bar - 1]) {
						board[foo][bar] += board[foo][bar];
						board[foo][bar - 1] = 0;
						score += board[foo][bar];
					}

				}
			}
			break;
		case dUP:
			for (int foo = 0; foo < 3; foo++) {
				for (int bar = 0; bar < 4; bar++) {
					if (board[foo][bar] == board[foo + 1][bar]) {
						board[foo][bar] += board[foo][bar];
						board[foo + 1][bar] = 0;
						score += board[foo][bar];
					}

				}
			}
			break;

		case dDOWN:
			for (int foo = 3; foo > 0; foo--) {
				for (int bar = 0; bar < 4; bar++) {
					if (board[foo][bar] == board[foo - 1][bar]) {
						board[foo][bar] += board[foo][bar];
						board[foo - 1][bar] = 0;
						score += board[foo][bar];
					}

				}
			}
	}
}

int moveTile(dtype dir) {

	int dummy[4][4], count = 0;

	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 4; b++) {
			dummy[a][b] = board[a][b];
		}
	}

	slide(dir);
	join(dir);
	slide(dir);

	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 4; b++) {
			if (dummy[a][b] == board[a][b])
				count++;
		}
	}
	if (count == 16)
		return 1;
	return 0;
}
void initBoard() {

	board = malloc(sizeof(int*) * 4);

	for (int foo = 0; foo < 4; foo++) {
		board[foo] = malloc(sizeof(int) * 4);
		for (int bar = 0; bar < 4; bar++) {
			board[foo][bar] = 0;
		}
	}
}

void spawnTile(WINDOW* wn) {

	/* Randomize starting position. */
	int x = rand() % 4, y = rand() % 4, z;

	/* Check for available tile. */
	while (board[x][y] != 0) {
		x = rand() % 4;
		y = rand() % 4;
	}

	/* Randomize tile's value with ratio 1:3 */
	z = (rand() & 3) ? 2 : 4;

	board[x][y] = z;
}

void drawBoard(WINDOW *wn) {

	int a = 0, b = 0, x_cor = 0, y_cor = 0;
	char num[5] = {};

	/* Refresh window. */
	werase(wn);

	/* Upper border. */
	waddch(wn, ACS_ULCORNER);
	for (a = 0; a < (width - 2); a++) {
		waddch(wn, ACS_HLINE);
	}
	a = 0;
	waddch(wn, ACS_URCORNER);

	while (a < (height - 5)) {

		/* Left side border. */
		waddch(wn, ACS_VLINE);

		if (a == 0 || a == 4 || a == 8 || a == 12) {

			for (b = 0; b < 4; b++) {

				sprintf(num, "%d", board[a/4][b]);
				wprintw(wn, "    ");
				getyx(wn, x_cor, y_cor);

				if (board[a/4][b] != 0 ) {
					int color = logOf2(board[a/4][b]);
					wattron(wn, COLOR_PAIR(color));
					mvwprintw(wn, a + 1, y_cor - strlen(num), "%d", board[a/4][b]);
					wattroff(wn, COLOR_PAIR(color));
				} else {
					mvwprintw(wn, a + 1, y_cor - strlen(num), " ");
				}

				if (b < 3)
					wprintw(wn, " ");
			}
		} else {
			for (int c = 0; c < 19; c++)
				wprintw(wn, " ");
		}

		/* Right side border. */
		waddch(wn, ACS_VLINE);
		a++;
	}

	/* Lower border. */
	waddch(wn, ACS_LLCORNER);
	for (a = 0; a < (width - 2); a++) {
		waddch(wn, ACS_HLINE);
	}
	waddch(wn, ACS_LRCORNER);
	mvwprintw(wn, height - 2, 0, "SCORE: %d", score);
	mvwprintw(wn, height - 1, 0, "HIGHSCORE: %d", hiscore);

}

void cleanUp() {

	for (int x = 0; x < 4; x++)
		free(board[x]);
	free(board);
}

int initColor() {
	if (!has_colors()) {
		return 1;
	} else {
		start_color();
		init_pair(0, 1, 0);
		init_pair(1, 2, 0);
		init_pair(2, 3, 0);
		init_pair(3, 4, 0);
		init_pair(4, 5, 0);
		init_pair(5, 6, 0);
		init_pair(6, 7, 0);
		init_pair(7, 1, 0);
		init_pair(8, 2, 0);
		init_pair(9, 3, 0);
		init_pair(10, 4, 0);
		init_pair(11, 5, 0);
		init_pair(12, 1, 2);
	}
	return 0;
}

void game(WINDOW* wn) {

	int move = 0;
	while (1) {

		if (move == 0) {
			spawnTile(wn);
			drawBoard(wn);
		}

		input:;
		char key = wgetch(wn);

		switch (key) {

		case 'w':
			move = moveTile(dUP);
			break;

		case 's':
			move = moveTile(dDOWN);
			break;

		case 'a':
			move = moveTile(dLEFT);
			break;

		case 'd':
			move = moveTile(dRIGHT);
			break;

		case 'q':
			return;
			break;

		default:
			goto input;
			break;
		}
	}

}

void highScore() {

}

void menu(WINDOW* wn) {

	char *startGame = "1. Start game", *hiScore = "2. High score";
	char choice = 'w', enter;

	werase(wn);
	while (1) {
		switch(choice){
			case 'w':
				wattron(wn, COLOR_PAIR(12));
				mvwprintw(wn, 0, 0,"%s", startGame);
				wattroff(wn, COLOR_PAIR(12));
				mvwprintw(wn, 1, 0,"%s", hiScore);
				break;
			case 's':
				mvwprintw(wn, 0, 0,"%s", startGame);
				wattron(wn, COLOR_PAIR(12));
				mvwprintw(wn, 1, 0,"%s", hiScore);
				wattroff(wn, COLOR_PAIR(12));
				break;
			case '\n':
				if (enter == 'w') {
					game(wn);
				} else {
					highScore();
				}
				break;
			case 'q':
				werase(wn);
				mvwprintw(wn, 0, 0, "Exitting...");
				wgetch(wn);
				cleanUp();
				echo();
				endwin();
				exit(0);
				break;

			default:
				break;

		}
		enter = choice;
		choice = wgetch(wn);

	}
}

int main(int argc, char **argv) {

	initscr();
	cbreak();
	noecho();
	curs_set(false);
	srand((unsigned int)time(NULL));

	boardSize = 4;
	initBoard();

	WINDOW *wn = newwin(height, width, 4, 30);

	if (initColor()) {
		wprintw(wn, "Terminal does not support color.\n");
		wgetch(wn);
	}

	menu(wn);
	return 0;
}
