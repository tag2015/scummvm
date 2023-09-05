#ifndef FALLOUT2_EXPORT_H
#define FALLOUT2_EXPORT_H

#include "fallout2/interpreter.h"

namespace Fallout2 {

int externalVariableSetValue(Program *program, const char *identifier, ProgramValue &value);
int externalVariableGetValue(Program *program, const char *name, ProgramValue &value);
int externalVariableCreate(Program *program, const char *identifier);
void _initExport();
void externalVariablesClear();
Program *externalProcedureGetProgram(const char *identifier, int *addressPtr, int *argumentCountPtr);
int externalProcedureCreate(Program *program, const char *identifier, int address, int argumentCount);
void _exportClearAllVariables();

} // namespace Fallout2

#endif /* EXPORT_H */
