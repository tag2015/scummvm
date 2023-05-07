#ifndef FALLOUT2_MEMORY_DEFS_H
#define FALLOUT2_MEMORY_DEFS_H

#include "common/system.h"

namespace Fallout2 {

typedef void *(MallocProc)(size_t size);
typedef void *(ReallocProc)(void *ptr, size_t newSize);
typedef void  (FreeProc)(void *ptr);

} // namespace Fallout2

#endif
