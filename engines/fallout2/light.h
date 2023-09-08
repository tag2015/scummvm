#ifndef FALLOUT2_LIGHT_H
#define FALLOUT2_LIGHT_H

namespace Fallout2 {

#define LIGHT_INTENSITY_MIN (65536 / 4)
#define LIGHT_INTENSITY_MAX 65536

typedef void AdjustLightIntensityProc(int elevation, int tile, int intensity);

int lightInit();
void lightReset();
void lightExit();
int lightGetAmbientIntensity();
void lightSetAmbientIntensity(int intensity, bool shouldUpdateScreen);
int lightGetTileIntensity(int elevation, int tile);
int lightGetTrueTileIntensity(int elevation, int tile);
void lightSetTileIntensity(int elevation, int tile, int intensity);
void lightIncreaseTileIntensity(int elevation, int tile, int intensity);
void lightDecreaseTileIntensity(int elevation, int tile, int intensity);
void lightResetTileIntensity();

} // namespace Fallout2

#endif
