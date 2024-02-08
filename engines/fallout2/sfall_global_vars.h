#ifndef FALLOUT2_SFALL_GLOBAL_VARS_H
#define FALLOUT2_SFALL_GLOBAL_VARS_H

#include "fallout2/db.h"

#include "common/scummsys.h"

namespace Fallout2 {

typedef int32 int32_t;
typedef uint32 uint32_t;
typedef uint64 uint64_t;

bool sfall_gl_vars_init();
void sfall_gl_vars_reset();
void sfall_gl_vars_exit();
bool sfall_gl_vars_save(File *stream);
bool sfall_gl_vars_load(File *stream);
bool sfall_gl_vars_store(const char *key, int value);
bool sfall_gl_vars_store(int key, int value);
bool sfall_gl_vars_fetch(const char *key, int &value);
bool sfall_gl_vars_fetch(int key, int &value);

} // namespace Fallout2

#endif
