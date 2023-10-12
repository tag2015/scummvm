#ifndef FALLOUT2_SOUND_EFFECTS_CACHE_H
#define FALLOUT2_SOUND_EFFECTS_CACHE_H

namespace Fallout2 {

// The maximum number of sound effects that can be loaded and played
// simultaneously.
#define SOUND_EFFECTS_MAX_COUNT (4)

int soundEffectsCacheInit(int cache_size, const char *effectsPath);
void soundEffectsCacheExit();
int soundEffectsCacheInitialized();
void soundEffectsCacheFlush();
int soundEffectsCacheFileOpen(const char *fname, int *sampleRate);
int soundEffectsCacheFileClose(int handle);
int soundEffectsCacheFileRead(int handle, void *buf, unsigned int size);
int soundEffectsCacheFileWrite(int handle, const void *buf, unsigned int size);
long soundEffectsCacheFileSeek(int handle, long offset, int origin);
long soundEffectsCacheFileTell(int handle);
long soundEffectsCacheFileLength(int handle);

} // namespace Fallout2

#endif
