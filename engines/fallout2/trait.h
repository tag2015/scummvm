#ifndef FALLOUT2_TRAIT_H
#define FALLOUT2_TRAIT_H

#include "fallout2/db.h"
#include "fallout2/trait_defs.h"

#include "common/savefile.h"

namespace Fallout2 {

int traitsInit();
void traitsReset();
void traitsExit();
int traitsLoad(File *stream);
int traitsLoadScumm(Common::InSaveFile *stream);
int traitsSave(Common::OutSaveFile *stream);
void traitsSetSelected(int trait1, int trait2);
void traitsGetSelected(int *trait1, int *trait2);
char *traitGetName(int trait);
char *traitGetDescription(int trait);
int traitGetFrmId(int trait);
bool traitIsSelected(int trait);
int traitGetStatModifier(int stat);
int traitGetSkillModifier(int skill);

} // namespace Fallout2

#endif
