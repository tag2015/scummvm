#ifndef FALLOUT2_DICTIONARY_H
#define FALLOUT2_DICTIONARY_H

#include "fallout2/memory_defs.h"

#include "common/file.h"

namespace Fallout2 {

typedef int(DictionaryReadProc)(Common::File *stream, void *buffer, unsigned int size, int a4);
typedef int(DictionaryWriteProc)(Common::File *stream, void *buffer, unsigned int size, int a4);

// NOTE: Last unnamed fields are likely seek, tell, and filelength.
typedef struct DictionaryIO {
	DictionaryReadProc *readProc;
	DictionaryWriteProc *writeProc;
	int field_8;
	int field_C;
	int field_10;
} DictionaryIO;

// A tuple containing individual key-value pair of a dictionary.
typedef struct DictionaryEntry {
	char *key;
	void *value;
} DictionaryEntry;

// A collection of key/value pairs.
//
// The keys in dictionary are always strings. Internally dictionary entries
// are kept sorted by the key. Both keys and values are copied when new entry
// is added to dictionary. For this reason the size of the value's type is
// provided during dictionary initialization.
typedef struct Dictionary {
	int marker;

	// The number of key/value pairs in the dictionary.
	int entriesLength;

	// The capacity of key/value pairs in [entries] array.
	int entriesCapacity;

	// The size of the dictionary values in bytes.
	size_t valueSize;

	// IO callbacks.
	DictionaryIO io;

	// The array of key-value pairs.
	DictionaryEntry *entries;
} Dictionary;

int dictionaryInit(Dictionary *dictionary, int initialCapacity, size_t valueSize, DictionaryIO *io);
int dictionarySetCapacity(Dictionary *dictionary, int newCapacity);
int dictionaryFree(Dictionary *dictionary);
int dictionaryGetIndexByKey(Dictionary *dictionary, const char *key);
int dictionaryAddValue(Dictionary *dictionary, const char *key, const void *value);
int dictionaryRemoveValue(Dictionary *dictionary, const char *key);
int dictionaryCopy(Dictionary *dest, Dictionary *src);
int dictionaryReadInt(Common::File *stream, int *valuePtr);
int dictionaryReadHeader(Common::File *stream, Dictionary *dictionary);
int dictionaryLoad(Common::File *stream, Dictionary *dictionary, int a3);
int dictionaryWriteInt(Common::File *stream, int value);
int dictionaryWriteHeader(Common::File *stream, Dictionary *dictionary);
int dictionaryWrite(Common::File *stream, Dictionary *dictionary, int a3);
void dictionarySetMemoryProcs(MallocProc *mallocProc, ReallocProc *reallocProc, FreeProc *freeProc);

} // namespace Fallout2

#endif
