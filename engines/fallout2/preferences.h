#ifndef FALLOUT2_PREFERENCES_H
#define FALLOUT2_PREFERENCES_H

#include "fallout2/db.h"

#include "common/savefile.h"

namespace Fallout2 {

int preferencesInit();
int doPreferences(bool animated);
int preferencesSave(Common::OutSaveFile *stream);
int preferencesLoad(Common::InSaveFile *stream);
void brightnessIncrease();
void brightnessDecrease();

} // namespace Fallout2

#endif
