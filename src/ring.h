#ifndef RING_H
#define RING_H

#include <stdbool.h>

typedef struct ring {
	int size;
	int location;
	char ascii[10];
	bool held;
} Ring;

extern Ring rings[3];
extern Ring *held;

void pickup_ring();

#endif
