#include <assert.h>
#include <stdlib.h>

#include "ring.h"
#include "tower.h"

void pickup_ring() {
	int i;
	int pickup = -1;

	// should not be called if we are already holding a ring
	assert(held == NULL);

	// find the smallest ring on the currently hovered tower (if there is one)
	for(i = 2; i >= 0; --i) {
		if(rings[i].location == hover) {
			pickup = i;
			break;
		}
	}

	if(pickup > 0) {
		held = &rings[pickup];
		held->held = true;
	}
}