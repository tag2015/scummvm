#ifndef FALLOUT2_WORD_WRAP_H
#define FALLOUT2_WORD_WRAP_H

namespace Fallout2 {

#define WORD_WRAP_MAX_COUNT (64)

int wordWrap(const char *string, int width, short *breakpoints, short *breakpointsLengthPtr);

} // namespace Fallout2

#endif
