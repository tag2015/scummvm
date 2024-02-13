#ifndef FALLOUT2_MOVIE_EFFECT_H
#define FALLOUT2_MOVIE_EFFECT_H

namespace Fallout2 {

int movieEffectsInit();
void movieEffectsReset();
void movieEffectsExit();
int movieEffectsLoad(const char *fileName);
void _moviefx_stop();

} // namespace Fallout2

#endif
