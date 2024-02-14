#include "fallout2/select_file_list.h"

/*#include <stdlib.h>
#include <string.h>*/

#include "fallout2/db.h"

namespace Fallout2 {

// 0x4AA250
int _compare(const void *a1, const void *a2) {
	void *a1_tmp = const_cast<void *>(a1);
	const char *v1 = *(static_cast<const char **>(a1_tmp));
	void *a2_tmp = const_cast<void *>(a2);
	const char *v2 = *(static_cast<const char **>(a2_tmp));
	return strcmp(v1, v2);
}

// 0x4AA2A4
char **_getFileList(const char *pattern, int *fileNameListLengthPtr) {
	char **fileNameList;
	int fileNameListLength = fileNameListInit(pattern, &fileNameList, 0, 0);
	*fileNameListLengthPtr = fileNameListLength;
	if (fileNameListLength == 0) {
		return nullptr;
	}

	qsort(fileNameList, fileNameListLength, sizeof(*fileNameList), _compare);

	return fileNameList;
}

// 0x4AA2DC
void _freeFileList(char **fileList) {
	fileNameListFree(&fileList, 0);
}

} // namespace Fallout2
