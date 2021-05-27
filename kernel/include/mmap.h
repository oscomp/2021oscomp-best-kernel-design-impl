#include "types.h"

struct mapped{
	int offset;
	uint64 ph_addr;
	int n_ref;
	struct mapped *next;
};