#ifndef FALLOUT2_MEMORY_H
#define FALLOUT2_MEMORY_H

#include "fallout2/memory_defs.h"

namespace Fallout2 {

char *internal_strdup(const char *string);
void *internal_malloc(size_t size);
void *internal_realloc(void *ptr, size_t size);
void internal_free(void *ptr);
void mem_check();

} // namespace Fallout2

#endif
