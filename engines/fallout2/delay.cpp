#include "common/system.h"

#include "fallout2/delay.h"

void delay_ms(int ms) {
	if (ms <= 0) {
		return;
	}
	g_system->delayMillis(ms);
}
