#ifndef FALLOUT2_POINTER_REGISTRY_H
#define FALLOUT2_POINTER_REGISTRY_H

// #include <unordered_map>
#include "fallout2/lib/std/map.h"

namespace Fallout2 {

class PointerRegistry {
public:
	static PointerRegistry *shared();

	PointerRegistry();

	int store(void *ptr);
	void *fetch(int ref, bool remove = false);

private:
	std::unordered_map<int, void *> _map;
	int _next;
};

int ptrToInt(void *ptr);
void *intToPtr(int ref, bool remove = false);

} // namespace Fallout2

#endif
