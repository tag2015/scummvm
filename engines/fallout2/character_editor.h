#ifndef FALLOUT2_CHARACTER_EDITOR_H
#define FALLOUT2_CHARACTER_EDITOR_H

#include "fallout2/db.h"

namespace Fallout2 {

extern int gCharacterEditorRemainingCharacterPoints;

int characterEditorShow(bool isCreationMode);
void characterEditorInit();
bool _isdoschar(int ch);
char *_strmfe(char *dest, const char *name, const char *ext);
int characterEditorSave(File *stream);
int characterEditorLoad(File *stream);
void characterEditorReset();

} // namespace Fallout2

#endif
