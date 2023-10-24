#include <stdlib.h>

#include "ring.h"
#include "tower.h"

int hover_max = 2;

void hover_move_right() {
	++hover;
	if(hover > hover_max) hover = hover_max;
	if(held != NULL) held->location = hover;
}

void hover_move_left() {
	--hover;
	if(hover < 0) hover = 0;
	if(held != NULL) held->location = hover;
}
