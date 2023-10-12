#include <curses.h>
#include <string.h>

#include "config.h"
#include "holder.h"
#include "ring.h"

void draw_title();
void draw_body();
void draw_holder(int ring_index, int startx, int starty);
void input_loop();

WINDOW *wmain;
WINDOW *wbody;
Ring rings[3];
Holder holders[3];
int hover = 0; // initially, hover over the leftmost holder

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
	}

	// initialize holders
	memset(holders[0].rings, true, 3);
	memset(holders[1].rings, false, 3);
	memset(holders[2].rings, false, 3);

	wmain = initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	// draw layout
	draw_title();
	wbody = newwin(7, getmaxx(wmain), 10, 0);
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
	draw_holder(0, getmaxx(wbody)/4 - 4, 6);
	draw_holder(1, getmaxx(wbody)/2 - 4, 6);
	draw_holder(2, 3*getmaxx(wbody)/4 - 4, 6);

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

void draw_holder(int ring_index, int startx, int starty) {
	int count;
	int i;

	// turn on bold attribute if tower is on hover
	if(ring_index == hover) wattron(wbody, A_BOLD);

	count = 4;
	mvwprintw(wbody, starty, startx, "---------");
	for(i = 0; i < 3; ++i) {
		if(holders[ring_index].rings[i]) {
			mvwprintw(wbody, getcury(wbody)-1, startx, rings[i].ascii);
			--count;
		}
	}
	while(count > 0) {
		mvwprintw(wbody, getcury(wbody)-1, startx, "    |    ");
		--count;
	}
	mvwprintw(wbody, getcury(wbody)-1, startx, "    ^    ");

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

		}
	}
}
