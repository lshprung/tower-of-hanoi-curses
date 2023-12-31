#include <curses.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"
#include "tower.h"
#include "ring.h"

#define TERMINAL_MIN_HEIGHT 14
#define TERMINAL_MIN_WIDTH  39

void init_rings();
void init_towers();
bool check_terminal();
void draw_title();
void draw_body();
void draw_tower(int ring_index, int startx, int starty);
void draw_win();
void draw_move_count();
void input_loop();

WINDOW *wmain;
WINDOW *wbody;
Ring rings[3];
Ring *held = NULL;
Tower towers[3];
int hover = 0; // initially, hover over the leftmost tower
int move_count = 0;

int main()
{
	init_rings();
	init_towers();

	wmain = initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	// check that the terminal can handle the program
	if(check_terminal()) {

		// draw title
		draw_title();
		refresh();

		// draw body
		wbody = newwin(12, getmaxx(wmain), getmaxy(wmain)/2 - 6, 0);
		draw_body();
		wrefresh(wbody);

		input_loop();
	}

	endwin();

	return 0;
}

void init_rings() {
	int i;

	// initialize rings
	strcpy(rings[0].ascii, " ======= ");
	strcpy(rings[1].ascii, "  =====  ");
	strcpy(rings[2].ascii, "   ===   ");
	for(i = 2; i >= 0; --i) {
		rings[i].size = i;
		rings[i].location = 0;
		rings[i].held = false;
	}
}

void init_towers() {
	// initialize towers
	memset(towers[0].rings, true, 3);
	memset(towers[1].rings, false, 3);
	memset(towers[2].rings, false, 3);
}

bool check_terminal() {
	// check that terminal is big enough
	if(getmaxx(wmain) < TERMINAL_MIN_WIDTH) return false;
	if(getmaxy(wmain) < TERMINAL_MIN_HEIGHT) return false;

	return true;
}

void draw_title() {
	move(0, getmaxx(wmain)/2 - strlen(TITLE)/2);
	printw(TITLE);
}

void draw_body() {
	werase(wbody);
	draw_tower(0, getmaxx(wbody)/4 - 4, 8);
	draw_tower(1, getmaxx(wbody)/2 - 4, 8);
	draw_tower(2, 3*getmaxx(wbody)/4 - 4, 8);
	draw_move_count();

	// FIXME placeholder implementation
	/*
	mvprintw(1, 0, "    ^    ");
	mvprintw(getcury(wmain)+1, 0, "    |    ");
	mvprintw(getcury(wmain)+1, 0, "    |    ");
	mvprintw(getcury(wmain)+1, 0, "    |    ");
	mvprintw(getcury(wmain)+1, 0, "    |    ");
	mvprintw(getcury(wmain)+1, 0, "---------");

	mvprintw(1, 10, "    ^    ");
	mvprintw(getcury(wmain)+1, 10, "    |    ");
	mvprintw(getcury(wmain)+1, 10, "   ===   ");
	mvprintw(getcury(wmain)+1, 10, "  =====  ");
	mvprintw(getcury(wmain)+1, 10, " ======= ");
	mvprintw(getcury(wmain)+1, 10, "---------");
	*/

	// DEBUG
	if(DEBUG) {
		mvprintw(0, 0, "tower 1: %c %c %c", (towers[0].rings[0] ? 'X' : '_'), (towers[0].rings[1] ? 'X' : '_'), (towers[0].rings[2] ? 'X' : '_'));
		mvprintw(1, 0, "tower 2: %c %c %c", (towers[1].rings[0] ? 'X' : '_'), (towers[1].rings[1] ? 'X' : '_'), (towers[1].rings[2] ? 'X' : '_'));
		mvprintw(2, 0, "tower 3: %c %c %c", (towers[2].rings[0] ? 'X' : '_'), (towers[2].rings[1] ? 'X' : '_'), (towers[2].rings[2] ? 'X' : '_'));
		mvprintw(3, 0, "ring 1:");
		mvprintw(4, 2, "location: %d", rings[0].location);
		mvprintw(5, 2, "held:     %d", rings[0].held);
		mvprintw(6, 2, "size:     %d", rings[0].size);
		mvprintw(7, 2, "ascii:    %s", rings[0].ascii);
		mvprintw(8, 0, "ring 2:");
		mvprintw(9, 2, "location: %d", rings[1].location);
		mvprintw(10, 2, "held:     %d", rings[1].held);
		mvprintw(11, 2, "size:     %d", rings[1].size);
		mvprintw(12, 2, "ascii:    %s", rings[1].ascii);
		mvprintw(13, 0, "ring 3:");
		mvprintw(14, 2, "location: %d", rings[2].location);
		mvprintw(15, 2, "held:     %d", rings[2].held);
		mvprintw(16, 2, "size:     %d", rings[2].size);
		mvprintw(17, 2, "ascii:    %s", rings[2].ascii);
	}	

	if(held == NULL && check_win_condition()) draw_win();
}

void draw_tower(int ring_index, int startx, int starty) {
	int count;
	int i;

	// turn on bold attribute if tower is on hover
	if(ring_index == hover) wattron(wbody, A_BOLD);

	count = 4;
	mvwprintw(wbody, starty, startx, "---------");
	for(i = 0; i < 3; ++i) {
		if(towers[ring_index].rings[i] && !rings[i].held) {
			mvwprintw(wbody, getcury(wbody)-1, startx, rings[i].ascii);
			--count;
		}
	}
	while(count > 0) {
		mvwprintw(wbody, getcury(wbody)-1, startx, "    |    ");
		--count;
	}
	mvwprintw(wbody, getcury(wbody)-1, startx, "    ^    ");

	// draw held ring above the others
	if(held != NULL && held->location == ring_index) {
		mvwprintw(wbody, getcury(wbody)-2, startx, held->ascii);
	}

	wattroff(wbody, A_BOLD);
}

void draw_win() {
	char *message = "WINNER (Return Key to play again)";
	mvwprintw(wbody, getmaxy(wbody)/2, getmaxx(wbody)/2 - strlen(message)/2, message);
}

void draw_move_count() {
	// tried to use log10, but this created weird problems
	int local_move_count = move_count;
	int count_length = 1;
	while(local_move_count >= 10) {
		local_move_count /= 10;
		++count_length;
	}
	mvwprintw(wbody, 10, getmaxx(wbody)/2 - (7 + count_length)/2,
			"moves: %d", move_count);
}

void input_loop() {
	int input = 0;

	while(input != 'q') {
		input = getch();

		if(held != NULL || !check_win_condition()) {
			switch(input) {
				case KEY_RIGHT:
					hover_move_right();
					draw_body();
					wrefresh(wbody);
					break;

				case KEY_LEFT:
					hover_move_left();
					draw_body();
					wrefresh(wbody);
					break;

				// pickup a ring
				case ' ': // spacebar
					if(held == NULL) {
						pickup_ring();
						if(held != NULL) ++move_count;
						draw_body();
						wrefresh(wbody);
					}
					else {
						if(drop_ring()) {
							draw_body();
							wrefresh(wbody);
						}
					}
					break;

			}
		}
		else {
			switch(input) {
				case 10: // enter/return key
					init_rings();
					init_towers();
					hover = 0;
					move_count = 0;
					draw_body();
					wrefresh(wbody);
					break;
			}
		}
	}
}
