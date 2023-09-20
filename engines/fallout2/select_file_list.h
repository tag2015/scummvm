#ifndef FALLOUT2_SELECT_FILE_LIST_H
#define FALLOUT2_SELECT_FILE_LIST_H

namespace Fallout2 {

int _compare(const void *a1, const void *a2);
char **_getFileList(const char *pattern, int *fileNameListLengthPtr);
void _freeFileList(char **fileList);

} // namespace Fallout2

#endif
