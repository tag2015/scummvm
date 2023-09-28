#ifndef FALLOUT2_ENDGAME_H
#define FALLOUT2_ENDGAME_H

namespace Fallout2 {

typedef enum EndgameDeathEndingReason {
	// Dude died.
	ENDGAME_DEATH_ENDING_REASON_DEATH = 0,

	// 13 years passed.
	ENDGAME_DEATH_ENDING_REASON_TIMEOUT = 2,
} EndgameDeathEndingReason;

extern char _aEnglish_2[];

void endgamePlaySlideshow();
void endgamePlayMovie();
int endgameDeathEndingInit();
int endgameDeathEndingExit();
void endgameSetupDeathEnding(int reason);
char *endgameDeathEndingGetFileName();

} // namespace Fallout2

#endif
