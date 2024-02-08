#include "fallout2/sfall_global_vars.h"

#include "fallout2/lib/std/map.h"

/*
#include <cstdint>
#include <cstring>
#include <unordered_map>
*/

namespace Fallout2 {

struct SfallGlobalVarsState {
	std::unordered_map<uint64_t, int> vars;
};

#pragma pack(push)
#pragma pack(8)

/// Matches Sfall's `GlobalVar` to maintain binary compatibility.
struct GlobalVarEntry {
	uint64_t key;
	int32_t value;
	int32_t unused;
};

#pragma pack(pop)

static bool sfall_gl_vars_store(uint64_t key, int value);
static bool sfall_gl_vars_fetch(uint64_t key, int &value);

static SfallGlobalVarsState *sfall_gl_vars_state = nullptr;

bool sfall_gl_vars_init() {
	sfall_gl_vars_state = new /*(std::nothrow)*/ SfallGlobalVarsState();
	if (sfall_gl_vars_state == nullptr) {
		return false;
	}

	return true;
}

void sfall_gl_vars_reset() {
	sfall_gl_vars_state->vars.clear();
}

void sfall_gl_vars_exit() {
	if (sfall_gl_vars_state != nullptr) {
		delete sfall_gl_vars_state;
		sfall_gl_vars_state = nullptr;
	}
}

bool sfall_gl_vars_save(File *stream) {
	// TODO loadsave
	warning("SFALL: Saving global vars not implemented");
	int count = static_cast<int>(sfall_gl_vars_state->vars.size());
	if (fileWrite(&count, sizeof(count), 1, stream) != 1) {
		return false;
	}

	GlobalVarEntry entry = {0, 0, 0};
	for (auto &pair : sfall_gl_vars_state->vars) {
		entry.key = pair._key;
		entry.value = pair._value;

		if (fileWrite(&entry, sizeof(entry), 1, stream) != 1) {
			return false;
		}
	}

	return true;
}

bool sfall_gl_vars_load(File *stream) {
	int count;
	if (fileRead(&count, sizeof(count), 1, stream) != 1) {
		return false;
	}

	sfall_gl_vars_state->vars.reserve(count);

	GlobalVarEntry entry;
	while (count > 0) {
		if (fileRead(&entry, sizeof(entry), 1, stream) != 1) {
			return false;
		}

		sfall_gl_vars_state->vars[entry.key] = static_cast<int>(entry.value);

		count--;
	}

	return true;
}

bool sfall_gl_vars_store(const char *key, int value) {
	if (strlen(key) != 8) {
		return false;
	}

	uint64_t numericKey = *(reinterpret_cast<const uint64_t *>(key));
	return sfall_gl_vars_store(numericKey, value);
}

bool sfall_gl_vars_store(int key, int value) {
	return sfall_gl_vars_store(static_cast<uint64_t>(key), value);
}

bool sfall_gl_vars_fetch(const char *key, int &value) {
	if (strlen(key) != 8) {
		return false;
	}

	uint64_t numericKey = *(reinterpret_cast<const uint64_t *>(key));
	return sfall_gl_vars_fetch(numericKey, value);
}

bool sfall_gl_vars_fetch(int key, int &value) {
	return sfall_gl_vars_fetch(static_cast<uint64_t>(key), value);
}

static bool sfall_gl_vars_store(uint64_t key, int value) {
	auto it = sfall_gl_vars_state->vars.find(key);
	if (it == sfall_gl_vars_state->vars.end()) {
		Fallout2::std::pair<Fallout2::uint64_t, int> newpair(key, value);
		sfall_gl_vars_state->vars.insert(newpair);
	} else {
		if (value == 0) {
			sfall_gl_vars_state->vars.erase(it);
		} else {
			it->_value = value;
		}
	}
	return true;
}

static bool sfall_gl_vars_fetch(uint64_t key, int &value) {
	auto it = sfall_gl_vars_state->vars.find(key);
	if (it == sfall_gl_vars_state->vars.end()) {
		return false;
	}

	value = it->_value;
	return true;
}

} // namespace Fallout2
