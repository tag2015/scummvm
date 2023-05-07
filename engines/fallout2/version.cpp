#include "fallout2/version.h"

namespace Fallout2 {

// 0x4B4580
void versionGetVersion(char *dest, size_t size) {
	snprintf(dest, size, "FALLOUT II %d.%02d", VERSION_MAJOR, VERSION_MINOR);
}

} // namespace Fallout2
