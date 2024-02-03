#include "fallout2/version.h"

#include "fallout2/sfall_config.h"

namespace Fallout2 {

// 0x4B4580
void versionGetVersion(char *dest, size_t size) {
	// SFALL: custom version string.
	char *versionString = nullptr;
	if (configGetString(&gSfallConfig, SFALL_CONFIG_MISC_KEY, SFALL_CONFIG_VERSION_STRING, &versionString)) {
		if (*versionString == '\0') {
			versionString = nullptr;
		}
	}
	snprintf(dest, size, (versionString != nullptr ? versionString : "FALLOUT II %d.%02d"), VERSION_MAJOR, VERSION_MINOR);
}

} // namespace Fallout2
