#ifndef FALLOUT2_TEXT_OBJECT_H
#define FALLOUT2_TEXT_OBJECT_H

#include "fallout2/geometry.h"
#include "fallout2/obj_types.h"

namespace Fallout2 {

int textObjectsInit(unsigned char *windowBuffer, int width, int height);
int textObjectsReset();
void textObjectsFree();
void textObjectsDisable();
void textObjectsEnable();
void textObjectsSetBaseDelay(double value);
void textObjectsSetLineDelay(double value);
int textObjectAdd(Object *object, char *string, int font, int color, int outlineColor, Rect *rect);
void textObjectsRenderInRect(Rect *rect);
int textObjectsGetCount();
void textObjectsRemoveByOwner(Object *object);

} // namespace Fallout2

#endif
