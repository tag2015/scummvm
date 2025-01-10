#ifndef FALLOUT2_GAME_MOVIE_H
#define FALLOUT2_GAME_MOVIE_H

#include "fallout2/db.h"

#include "common/savefile.h"

namespace Fallout2 {

typedef enum GameMovieFlags {
	GAME_MOVIE_FADE_IN = 0x01,
	GAME_MOVIE_FADE_OUT = 0x02,
	GAME_MOVIE_STOP_MUSIC = 0x04,
	GAME_MOVIE_PAUSE_MUSIC = 0x08,
} GameMovieFlags;

typedef enum GameMovie {
	MOVIE_IPLOGO,
	MOVIE_INTRO,
	MOVIE_ELDER,
	MOVIE_VSUIT,
	MOVIE_AFAILED,
	MOVIE_ADESTROY,
	MOVIE_CAR,
	MOVIE_CARTUCCI,
	MOVIE_TIMEOUT,
	MOVIE_TANKER,
	MOVIE_ENCLAVE,
	MOVIE_DERRICK,
	MOVIE_ARTIMER1,
	MOVIE_ARTIMER2,
	MOVIE_ARTIMER3,
	MOVIE_ARTIMER4,
	MOVIE_CREDITS,
	MOVIE_COUNT,
} GameMovie;

int gameMoviesInit();
void gameMoviesReset();
int gameMoviesLoad(Common::InSaveFile *stream);
int gameMoviesSave(Common::OutSaveFile *stream);
int gameMoviePlay(int movie, int flags);
void gameMovieFadeOut();
bool gameMovieIsSeen(int movie);
bool gameMovieIsPlaying();

} // namespace Fallout2

#endif
