#include <stdlib.h>

/// @brief Used for managing allocations internally.
typedef struct _Allocation {
	size_t 	size;
	bool 	free;

	struct _Allocation *next;
} Allocation;

class Heap {
	private:
		Allocation *base;

	public:
	Heap(void *start, size_t size) {
		if(size <= sizeof(Allocation))
		base = (Allocation *)start;
		base->size = size - sizeof(Allocation);
		base->free = true;
		base->next = NULL;
	}
};