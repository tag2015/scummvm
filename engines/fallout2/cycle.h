#ifndef FALLOUT2_CYCLE_H
#define FALLOUT2_CYCLE_H

namespace Fallout2 {

void colorCycleInit();
void colorCycleReset();
void colorCycleFree();
void colorCycleDisable();
void colorCycleEnable();
bool colorCycleEnabled();
void cycleSetSpeedFactor(int value);
void colorCycleTicker();

unsigned int getTicksBetween(unsigned int end, unsigned int start); // TODO: remove

} // namespace Fallout2

#endif
