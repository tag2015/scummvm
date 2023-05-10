#ifndef FALLOUT2_RANDOM_H
#define FALLOUT2_RANDOM_H

// #include "db.h"  TODO

namespace Fallout2 {

typedef enum Roll {
	ROLL_CRITICAL_FAILURE,
	ROLL_FAILURE,
	ROLL_SUCCESS,
	ROLL_CRITICAL_SUCCESS,
} Roll;

void randomInit();
void randomReset();
void randomExit();
// int randomSave(File* stream);  TODO
// int randomLoad(File* stream);  TODO
int randomRoll(int difficulty, int criticalSuccessModifier, int *howMuchPtr);
int randomBetween(int min, int max);
void randomSeedPrerandom(int seed);

} // namespace Fallout2

#endif
