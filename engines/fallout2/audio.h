#ifndef FALLOUT2_AUDIO_H
#define FALLOUT2_AUDIO_H

namespace Fallout2 {

typedef bool(AudioQueryCompressedFunc)(char *filePath);

int audioOpen(const char *fname, int *sampleRate);
int audioClose(int handle);
int audioRead(int handle, void *buffer, unsigned int size);
long audioSeek(int handle, long offset, int origin);
long audioGetSize(int handle);
long audioTell(int handle);
int audioWrite(int handle, const void *buf, unsigned int size);
int audioInit(AudioQueryCompressedFunc *func);
void audioExit();

} // namespace Fallout2

#endif
