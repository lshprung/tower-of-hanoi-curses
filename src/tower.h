#ifndef TOWER_H
#define TOWER_H

#include <stdbool.h>

typedef struct tower {
	bool rings[3];
} Tower;

extern Tower towers[3];
extern int hover;

void hover_move_right();
void hover_move_left();

#endif
