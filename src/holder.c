#include "holder.h"

int hover_max = 2;

void hover_move_right() {
	++hover;
	if(hover > hover_max) hover = hover_max;
}

void hover_move_left() {
	--hover;
	if(hover < 0) hover = 0;
}
