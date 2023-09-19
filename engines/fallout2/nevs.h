#ifndef FALLOUT2_NEVS_H
#define FALLOUT2_NEVS_H

#include "fallout2/interpreter.h"

namespace Fallout2 {

typedef enum NevsType {
	NEVS_TYPE_EVENT = 0,
	NEVS_TYPE_HANDLER = 1,
} NevsType;

void _nevs_close();
void _nevs_initonce();
int _nevs_addevent(const char *name, Program *program, int proc, int type);
int _nevs_clearevent(const char *name);
int _nevs_signal(const char *name);
void _nevs_update();

} // namespace Fallout2

#endif
