#ifndef FALLOUT2_INTERPRETER_LIB_H
#define FALLOUT2_INTERPRETER_LIB_H

#include "fallout2/interpreter.h"

namespace Fallout2 {

typedef void(IntLibProgramDeleteCallback)(Program *);

void intLibUpdate();
void intLibExit();
void intLibInit();
void intLibRegisterProgramDeleteCallback(IntLibProgramDeleteCallback *callback);
void intLibRemoveProgramReferences(Program *program);

} // namespace Fallout2

#endif
