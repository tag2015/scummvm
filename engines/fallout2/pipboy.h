#ifndef FALLOUT2_PIPBOY_H
#define FALLOUT2_PIPBOY_H

#include "fallout2/db.h"

namespace Fallout2 {

typedef enum PipboyOpenIntent {
	PIPBOY_OPEN_INTENT_UNSPECIFIED = 0,
	PIPBOY_OPEN_INTENT_REST = 1,
} PipboyOpenIntent;

int pipboyOpen(int intent);
void pipboyInit();
void pipboyReset();
int pipboySave(File *stream);
int pipboyLoad(File *stream);

} // namespace Fallout2

#endif
