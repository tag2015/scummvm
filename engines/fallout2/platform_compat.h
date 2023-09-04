#ifndef FALLOUT2_PLATFORM_COMPAT_H
#define FALLOUT2_PLATFORM_COMPAT_H

#include "common/compression/deflate.h"
#include "common/file.h"
#include "common/system.h"

namespace Fallout2 {

// TODO: This is compatibility cross-platform layer. Designed to have minimal
// impact on the codebase. Remove once it's no longer needed.

// A naive cross-platform MAX_PATH/PATH_MAX/MAX_PATH drop-in replacement.
//
// TODO: Remove when we migrate to use std::filesystem::path or std::string to
// represent paths across the codebase.
#define COMPAT_MAX_PATH 260

#define COMPAT_MAX_DRIVE 3
#define COMPAT_MAX_DIR 256
#define COMPAT_MAX_FNAME 256
#define COMPAT_MAX_EXT 256

int compat_stricmp(const char *string1, const char *string2);
int compat_strnicmp(const char *string1, const char *string2, size_t size);
char *compat_strupr(char *string);
char *compat_strlwr(char *string);
char *compat_itoa(int value, char *buffer, int radix);
void compat_splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);
void compat_makepath(char *path, const char *drive, const char *dir, const char *fname, const char *ext);
// long compat_tell(int fileHandle);
// long compat_filelength(int fd);
int compat_mkdir(const char *path);
unsigned int compat_timeGetTime();
Common::File *compat_fopen(const char *path, const char *mode);
// gzFile compat_gzopen(const char *path, const char *mode);
char *compat_fgets(char *buffer, int maxCount, Common::File *stream);
char *compat_gzgets(Common::SeekableReadStream *stream, char *buffer, int maxCount);
int compat_remove(const char *path);
int compat_rename(const char *oldFileName, const char *newFileName);
void compat_windows_path_to_native(char *path);
void compat_resolve_path(char *path);
int compat_access(const char *path, int mode);
char *compat_strdup(const char *string);
long getFileSize(Common::File *stream);

} // namespace Fallout2

#endif
