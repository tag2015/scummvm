#ifndef FALLOUT2_MEMORY_MANAGER_H
#define FALLOUT2_MEMORY_MANAGER_H

#include "fallout2/memory_defs.h"

namespace Fallout2 {

void memoryManagerSetProcs(MallocProc *mallocProc, ReallocProc *reallocProc, FreeProc *freeProc);
void *internal_malloc_safe(size_t size, const char *file, int line);
void *internal_realloc_safe(void *ptr, size_t size, const char *file, int line);
void internal_free_safe(void *ptr, const char *file, int line);
void *internal_calloc_safe(int count, int size, const char *file, int line);
char *strdup_safe(const char *string, const char *file, int line);

} // namespace Fallout2

#endif
