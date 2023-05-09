#include "fallout2/file_find.h"

#include "third_party/fpattern/fpattern.h"

/*#include <stddef.h>
#include <string.h>
*/

namespace Fallout2 {

// 0x4E6380
bool fileFindFirst(const char *path, DirectoryFileFindData *findData) {
	/*#if defined(_WIN32)
		findData->hFind = FindFirstFileA(path, &(findData->ffd));
		if (findData->hFind == INVALID_HANDLE_VALUE) {
			return false;
		}
	#else*/
	strncpy(findData->path, path, sizeof(findData->path) - 1);

	char drive[COMPAT_MAX_DRIVE];
	char dir[COMPAT_MAX_DIR];
	compat_splitpath(path, drive, dir, NULL, NULL);

	char basePath[COMPAT_MAX_PATH];
	compat_makepath(basePath, drive, dir, NULL, NULL);

	findData->dir = Common::FSNode(basePath);
	if (!findData->dir.exists())
		return false;

	Common::FSList fslist;
	findData->dir.getChildren(fslist, Common::FSNode::kListFilesOnly);

	bool found = false;
	for (Common::FSList::const_iterator file = fslist.begin(); !found && file != fslist.end(); ++file) {
		char entryPath[COMPAT_MAX_PATH];
		compat_makepath(entryPath, drive, dir, file->getName().c_str(), NULL);
		if (fpattern_match(findData->path, entryPath)) {
			found = true;
			findData->entry = Common::FSNode(file->getPath());
		}
	}

	if (!found) {
		findData->found = false;
		return false;
	}

	/*	findData->dir = opendir(basePath);
		if (findData->dir == NULL) {
			return false;
		}

		findData->entry = readdir(findData->dir);
		while (findData->entry != NULL) {
			char entryPath[COMPAT_MAX_PATH];
			compat_makepath(entryPath, drive, dir, fileFindGetName(findData), NULL);
			if (fpattern_match(findData->path, entryPath)) {
				break;
			}
			findData->entry = readdir(findData->dir);
		}

		if (findData->entry == NULL) {
			closedir(findData->dir);
			findData->dir = NULL;
			return false;
		}
		// #endif */

	findData->found = true;
	return true;
}

// 0x4E63A8
bool fileFindNext(DirectoryFileFindData *findData) {
	// #if defined(_WIN32)
	//	if (!FindNextFileA(findData->hFind, &(findData->ffd))) {
	//		return false;
	//	}
	// #else

	if (!findData->found)
		return false;

	char drive[COMPAT_MAX_DRIVE];
	char dir[COMPAT_MAX_DIR];
	compat_splitpath(findData->path, drive, dir, NULL, NULL);

	Common::FSList fslist;
	findData->dir.getChildren(fslist, Common::FSNode::kListFilesOnly);

	bool found = false;
	for (Common::FSList::const_iterator file = fslist.begin(); !found && file != fslist.end(); ++file) {
		char entryPath[COMPAT_MAX_PATH];
		compat_makepath(entryPath, drive, dir, file->getName().c_str(), NULL);
		if (fpattern_match(findData->path, entryPath)) {
			found = true;
			findData->entry = Common::FSNode(file->getPath());
		}
	}

	if (!found) {
		findData->found = false;
		return false;
	}

	/*	findData->entry = readdir(findData->dir);
		while (findData->entry != NULL) {
			char entryPath[COMPAT_MAX_PATH];
			compat_makepath(entryPath, drive, dir, fileFindGetName(findData), NULL);
			if (fpattern_match(findData->path, entryPath)) {
				break;
			}
			findData->entry = readdir(findData->dir);
		}

		if (findData->entry == NULL) {
			closedir(findData->dir);
			findData->dir = NULL;
			return false;
		} */
	// #endif
	findData->found = true;
	return true;
}

// 0x4E63CC
bool findFindClose(DirectoryFileFindData *findData) {
	// TODO!!!
	// #if defined(_MSC_VER)
	//	FindClose(findData->hFind);
	// #else
	//	if (findData->dir != NULL) {
	// if (closedir(findData->dir) != 0) {
	// findData->dir = NULL;
	//	return false;
	//}
	// #endif

	return true;
}

} // namespace Fallout2
