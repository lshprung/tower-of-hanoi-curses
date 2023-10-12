#ifndef HOLDER_H
#define HOLDER_H

#include <stdbool.h>

extern int hover;

void hover_move_right();
void hover_move_left();

typedef struct holder {
	bool rings[3];
} Holder;

#endif
