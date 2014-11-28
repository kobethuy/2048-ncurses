/**
 * 	2048.c
 *
 *  Created on: Nov 2, 2014
 *  Author: kobethuy
 */

#include <curses.h> 	// GUI
#include <time.h> 		// time(), localtime(), struct tm
#include <stdlib.h> 	// malloc()
#include <unistd.h> 	// getopt()
#include <strings.h> 	// strlen()

/* Direction. */
typedef enum {
	dUP,
	dDOWN,
	dLEFT,
	dRIGHT
} dtype;

struct Highscore {
	char *name;
	int score;
	char *time;
};

char *usage = 		"Use \"w,a,s,d\" to move tiles, \"q\" to exit.",							\
	 *menuOpt[5] = {"Start game", "High scores", "Credits", "Quit", "Continue"},				\
	 *member = 		"Name: Nguyen Trung Dang Thuy\nID: s3509547\nEmail: s3509547@rmit.edu.vn";

struct Highscore hs[10]; // High score list.

int **board,		\
	boardSize,		\
	width = 21,		\
	height = 20,	\
	score = 0,		\
	hiscore = 0,	\
	winGoal = 2048,	\
	start = 0,		\
	win = 0,		\
	usrRetry = 0;

/**
 * Function prototypes.
 *
 */

void cleanUp();
void initHiScore(struct Highscore hs[]);
void getHiScore(struct Highscore hs[]);
void setHiScore(struct Highscore hs[]);

/**
 * Return the power of 2 of a given number.
 *
 * @param a The number to compute.
 * @return	2 ^ a.
 */
int powerOf2(int a) {

	if (a == 0) {
		return 1;
	} else {
		return 2 * powerOf2(a - 1);
	}
}

/**
 * Return the logarithm of a number.
 *
 * @param a The number to compute.
 * @return log2(a).
 */
int logOf2(int a) {
	int foo = 0;
	while ((powerOf2(foo)) != a)
		foo++;
	return foo;
}

/**
 * Check for winning condition.
 *
 * @return 1 if there exist a tile that match the winning goal. 0 otherwise.
 */
int winCheck() {
	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 4; b++) {
			if (board[a][b] == winGoal)
				return 1;
		}
	}
	return 0;
}


/**
 * Check for available move on the board.
 *
 * @return 1 if there is an available move. 0 otherwise.
 */
int checkMove() {
    int i, j;
    for (i = 0; i < 4; i++) {
    	for (j = 0; j < 4; j++) {
    		if (!board[i][j]
						  || ((i + 1 < 4) && (board[i][j] == board[i + 1][j]))
						  || ((j + 1 < 4) && (board[i][j] == board[i][j + 1])))
    			return 1;
    	}
    }
    return 0;
}

/**
 * Slide all tile in the given direction.
 *
 * @param dir The direction to slide.
 */
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

/**
 * Join tiles in given direction.
 *
 * @param dir The direction to join.
 */
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

/**
 * General function to simulate a "move".
 *
 * @param dir The direction to move.
 * @return 1 if there if no available move. 0 otherwise.
 */
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

	if (winCheck())
		win = 1;

	if (count == 16)
		return 1;

	return 0;
}

/**
 * Initialize the game board.
 */
void initBoard() {

	board = malloc(sizeof(int*) * 4);

	for (int foo = 0; foo < 4; foo++) {
		board[foo] = malloc(sizeof(int) * 4);
		for (int bar = 0; bar < 4; bar++) {
			board[foo][bar] = 0;
		}
	}
}

/**
 * Generate random tile.
 */
void spawnTile() {

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

/**
 * Draw the gameboard.
 *
 * @param wn The active window to draw.
 */
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

	while (a < (height - 7)) {

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
	getHiScore(hs);
	hiscore = hs[0].score;
	mvwprintw(wn, height - 1, 0, "HIGHSCORE: %d", hiscore);

}

/**
 * Clear the active window.
 *
 * @param wn The active window to clear.
 */
void clearWn(WINDOW* wn) {
	wgetch(wn);
	werase(wn);
	mvwprintw(wn, 0, 0, "Press any key...");
	wgetch(wn);
	werase(wn);
}

/**
 * View the highscore.
 */
void viewHiScore() {

	WINDOW* wnHiScore = newwin(24, 80, 4, 30);

	/* Proper formatting */
	mvwprintw(wnHiScore, 0, 0, "#");
	mvwprintw(wnHiScore, 0, 3, "Name");
	mvwprintw(wnHiScore, 0, 14, "Score");
	mvwprintw(wnHiScore, 0, 25, "Time");
	for (int i = 0; i < 10; i++) {
		mvwprintw(wnHiScore, i + 1, 0, "%d", i + 1);
		mvwprintw(wnHiScore, i + 1, 2, ".");
		mvwprintw(wnHiScore, i + 1, 3, "%s", hs[i].name);
		mvwprintw(wnHiScore, i + 1, 14, "%d", hs[i].score);
		mvwprintw(wnHiScore, i + 1, 25, "%s", hs[i].time);
	}
	/*-------------------*/

	clearWn(wnHiScore);
	endwin();
}

/**
 * View the credit.
 */
void viewCredit() {

	WINDOW* wnCredit = newwin(24, 80, 4, 30);
	wprintw(wnCredit, "%s", member);
	clearWn(wnCredit);
	endwin();
}

/**
 * Store highscore in formatted form.
 *
 * @param score The score to store.
 */
void storeHiScore(int score) {

	char *name = malloc(sizeof(char) * 11), *timeLocal = malloc(sizeof(char) * 17);

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	WINDOW* wnStoreHS = newwin(24, 80, 4, 30);

	ret:;
	mvwprintw(wnStoreHS, 0, 0, "Please enter your name: ");
	wscanw(wnStoreHS, "%s", name);

	if (strlen(name) == 0 || strlen(name) > 10)
		goto ret;

	sprintf(timeLocal, "%d:%d_%d/%d/%d", tm.tm_hour, tm.tm_min, tm.tm_mday, tm.tm_mon, tm.tm_year + 1900);

	if (hs[9].score < score) {
		hs[9].name = name;
		hs[9].score = score;
		hs[9].time = timeLocal;
	}

	setHiScore(hs);
	clearWn(wnStoreHS);
	endwin();
}

/**
 * Game over window.
 *
 * @param wn The active window to show.
 * @return 1 if the player doesn't retry. 0 otherwise.
 */
int gameOver(WINDOW* wn) {


	mvwprintw(wn, 1, 0, "Try again ? Y/N");

	ret:;
	char retry = wgetch(wn);

	if (retry == 'y' || retry == 'Y') {
		score = 0;
		start = 0;
		return 0;
	} else if (retry == 'n' || retry == 'N'){
		werase(wn);
		storeHiScore(score);
		return 1;
	} else {
		werase(wn);
		goto ret;
	}

}

/**
 * Game manager.
 *
 * @param wn The game window.
 * @return -1 if the player quit the game. 1 if not retry. 0 otherwise.
 */
int game(WINDOW* wn) {

	int move = 0;
	while (1) {

		if (move == 0) {
			if (start == 0 || start == 1)
				spawnTile();
			drawBoard(wn);
		}

		input:;
		int key = wgetch(wn);

		switch (key) {

		case KEY_UP:
			move = moveTile(dUP);
			break;

		case KEY_DOWN:
			move = moveTile(dDOWN);
			break;

		case KEY_LEFT:
			move = moveTile(dLEFT);
			break;

		case KEY_RIGHT:
			move = moveTile(dRIGHT);
			break;

		case 'q':
		case 27:
			return -1;
			break;

		default:
			goto input;
			break;
		}

		if (win) {
			wgetch(wn);

			werase(wn);

			mvwprintw(wn, 0, 0, "YOU WIN !!!");

			wgetch(wn);
			if (gameOver(wn)) {
				cleanUp();
				initBoard();
				return 0;
			}
			return 1;

		} else if (!checkMove()) {
			wgetch(wn);

			werase(wn);

			mvwprintw(wn, 0, 0, "YOU LOSE !!!");

			wgetch(wn);

			if (gameOver(wn)) {
				cleanUp();
				initBoard();
				return 0;
			}
			return 1;
		}
	}

}

/**
 * Menu manager.
 *
 * @param wn The menu window.
 */
void menu(WINDOW* wn) {

	int index = 0;
	int choice = 0;
	werase(wn);

	while (1) {

		switch(choice) {

			case KEY_UP:
				if (index == 0)
					break;
				index--;
				break;

			case KEY_DOWN:
				if (index == 3)
					break;
				index++;
				break;

			case '\n':
				switch (index) {
					case 0:
						switch(game(wn)) {
							case -1:
								start = 1;
								break;

							case 0:
								start = 0;
								score = 0;
								game(wn);
								break;

							case 1:
								break;
						}
						werase(wn);
						break;

					case 1:
						werase(wn);
						getHiScore(hs);
						viewHiScore();
						break;

					case 2:
						werase(wn);
						viewCredit();
						break;

					case 3:
						werase(wn);
						wprintw(wn, "Are you sure ? Y/N");

						ret:;
						int exitChoice = wgetch(wn);
						switch(exitChoice) {
							case 'Y':
							case 'y':
								werase(wn);
								if (score > hs[9].score)
									storeHiScore(score);
								werase(wn);
								mvwprintw(wn, 0, 0, "Goodbye!\nPress any key...");
								wgetch(wn);
								cleanUp();
								echo();
								endwin();
								exit(0);
								break;
							case 'N':
							case 'n':
								werase(wn);
								break;
							default:
								goto ret;
								break;
						}

				}
				break;
		}

		for (int foo = 0; foo < 4; foo++) {
			if (foo == index) {
				wattron(wn, COLOR_PAIR(12));
				if (foo == 0 && start == 1) {
					mvwprintw(wn, foo, 0, "%s", menuOpt[4]);
				} else {
					mvwprintw(wn, foo, 0, "%s", menuOpt[foo]);
				}
				wattroff(wn, COLOR_PAIR(12));
			} else {
				if (foo == 0 && start == 1) {
					mvwprintw(wn, foo, 0, "%s", menuOpt[4]);
				} else {
					mvwprintw(wn, foo, 0, "%s", menuOpt[foo]);
				}
			}
		}

		choice = wgetch(wn);
	}
}

/**
 * Cleanup any memory leaks.
 */
void cleanUp() {

	for (int x = 0; x < 4; x++)
		free(board[x]);
	free(board);
}

/**
 * Initialize color.
 *
 * @return 1 if terminal doesn't support color. 0 otherwise.
 */
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

/**
 * Main entry point.
 *
 * @param argc The number of CMD arguments.
 * @param argv The CMD arguments.
 * @return 1 if there's an error. 0 otherwise.
 */
int main(int argc, char **argv) {

	int args = getopt(argc, argv, "hw:");

	initHiScore(hs);

	switch (args) {
		case 'h':
			printf("%s\n", usage);
			exit(0);
			break;
		case 'w':
			winGoal = atoi(optarg);
			break;

	}

	initscr();
	cbreak();
	echo();
	curs_set(false);

	srand((unsigned int)time(NULL));

	hiscore = 0;

	boardSize = 4;
	initBoard();

	WINDOW *wn = newwin(height, width, 4, 30);
	keypad(wn, TRUE);

	if (initColor()) {
		wprintw(wn, "Terminal does not support color.\n");
		wgetch(wn);
	}

	menu(wn);
	return 0;
}
