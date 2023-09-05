#ifndef FALLOUT2_INTERPRETER_EXTRA_H
#define FALLOUT2_INTERPRETER_EXTRA_H

#include "fallout2/interpreter.h"

namespace Fallout2 {

void _intExtraClose_();
void _initIntExtra();
void intExtraUpdate();
void intExtraRemoveProgramReferences(Program *program);

} // namespace Fallout2

#endif
