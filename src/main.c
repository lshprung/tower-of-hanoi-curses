#include <curses.h>
#include <string.h>

#include "config.h"
#include "tower.h"
#include "ring.h"

void draw_title();
void draw_body();
void draw_tower(int ring_index, int startx, int starty);
void input_loop();

WINDOW *wmain;
WINDOW *wbody;
Ring rings[3];
Ring *held = NULL;
Tower towers[3];
int hover = 0; // initially, hover over the leftmost tower

int main()
{
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

	// initialize towers
	memset(towers[0].rings, true, 3);
	memset(towers[1].rings, false, 3);
	memset(towers[2].rings, false, 3);

	wmain = initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	// draw layout
	draw_title();
	wbody = newwin(9, getmaxx(wmain), 8, 0);
	draw_body();

	refresh();
	wrefresh(wbody);

	input_loop();

	endwin();

	return 0;
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

void input_loop() {
	int input = 0;

	while(input != 'q') {
		input = getch();

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
				// TODO check if a ring is already held, in which case we need to do something else
				pickup_ring();
				draw_body();
				wrefresh(wbody);
				break;

		}
	}
}
