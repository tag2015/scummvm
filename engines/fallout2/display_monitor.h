#ifndef FALLOUT2_DISPLAY_MONITOR_H
#define FALLOUT2_DISPLAY_MONITOR_H

namespace Fallout2 {

int displayMonitorInit();
int displayMonitorReset();
void displayMonitorExit();
void displayMonitorAddMessage(char *string);
void displayMonitorDisable();
void displayMonitorEnable();

} // namespace Fallout2

#endif
