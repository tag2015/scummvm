#ifndef FALLOUT2_PREFERENCES_H
#define FALLOUT2_PREFERENCES_H

#include "fallout2/db.h"

namespace Fallout2 {

int preferencesInit();
int doPreferences(bool animated);
int preferencesSave(File *stream);
int preferencesLoad(File *stream);
void brightnessIncrease();
void brightnessDecrease();

} // namespace Fallout2

#endif
