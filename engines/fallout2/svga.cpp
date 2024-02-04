#include "fallout2/svga.h"

/*#include <limits.h>
#include <string.h>

#include <SDL.h>*/

#include "fallout2/fallout2.h"
#include "fallout2/config.h"
#include "fallout2/draw.h"
#include "fallout2/interface.h"
#include "fallout2/memory.h"
#include "fallout2/mouse.h"
#include "fallout2/win32.h"
#include "fallout2/window_manager.h"
#include "fallout2/window_manager_private.h"

#include "common/rect.h"
#include "graphics/palette.h"

namespace Fallout2 {

static bool createRenderer(int width, int height);
static void destroyRenderer();

// screen rect
Rect _scr_size;

// 0x6ACA18
void (*_scr_blit)(unsigned char *src, int src_pitch, int a3, int src_x, int src_y, int src_width, int src_height, int dest_x, int dest_y) = _GNW95_ShowRect;

// 0x6ACA1C
void (*_zero_mem)() = nullptr;

/*SDL_Window *gSdlWindow = nullptr;
SDL_Surface *gSdlSurface = nullptr;
SDL_Renderer *gSdlRenderer = nullptr;
SDL_Texture *gSdlTexture = nullptr;
SDL_Surface *gSdlTextureSurface = nullptr;
*/

Graphics::Surface *gSdlSurface;
Graphics::ManagedSurface *gSdlTexture;
Graphics::ManagedSurface *gSdlTextureSurface;

// TODO: Remove once migration to update-render cycle is completed.
FpsLimiter sharedFpsLimiter;

// 0x4CAD08
int _init_mode_320_200() {
	return _GNW95_init_mode_ex(320, 200, 8);
}

// 0x4CAD40
int _init_mode_320_400() {
	return _GNW95_init_mode_ex(320, 400, 8);
}

// 0x4CAD5C
int _init_mode_640_480_16() {
	return -1;
}

// 0x4CAD64
int _init_mode_640_480() {
	return _init_vesa_mode(640, 480);
}

// 0x4CAD94
int _init_mode_640_400() {
	return _init_vesa_mode(640, 400);
}

// 0x4CADA8
int _init_mode_800_600() {
	return _init_vesa_mode(800, 600);
}

// 0x4CADBC
int _init_mode_1024_768() {
	return _init_vesa_mode(1024, 768);
}

// 0x4CADD0
int _init_mode_1280_1024() {
	return _init_vesa_mode(1280, 1024);
}

// 0x4CADF8
void _get_start_mode_() {
}

// 0x4CADFC
void _zero_vid_mem() {
	if (_zero_mem) {
		_zero_mem();
	}
}

// 0x4CAE1C
int _GNW95_init_mode_ex(int width, int height, int bpp) {
	bool fullscreen = true;
	int scale = 1;

	/*	Config resolutionConfig; TODO read config
		if (configInit(&resolutionConfig)) {
			if (configRead(&resolutionConfig, "f2_res.ini", false)) {
				int screenWidth;
				if (configGetInt(&resolutionConfig, "MAIN", "SCR_WIDTH", &screenWidth)) {
					width = screenWidth;
				}

				int screenHeight;
				if (configGetInt(&resolutionConfig, "MAIN", "SCR_HEIGHT", &screenHeight)) {
					height = screenHeight;
				}

				bool windowed;
				if (configGetBool(&resolutionConfig, "MAIN", "WINDOWED", &windowed)) {
					fullscreen = !windowed;
				}

				int scaleValue;
				if (configGetInt(&resolutionConfig, "MAIN", "SCALE_2X", &scaleValue)) {
					scale = scaleValue + 1; // 0 = 1x, 1 = 2x
					// Only allow scaling if resulting game resolution is >= 640x480
					if ((width / scale) < 640 || (height / scale) < 480) {
						scale = 1;
					} else {
						width /= scale;
						height /= scale;
					}
				}

				configGetBool(&resolutionConfig, "IFACE", "IFACE_BAR_MODE", &gInterfaceBarMode);
				configGetInt(&resolutionConfig, "IFACE", "IFACE_BAR_WIDTH", &gInterfaceBarWidth);
				configGetInt(&resolutionConfig, "IFACE", "IFACE_BAR_SIDE_ART", &gInterfaceSidePanelsImageId);
				configGetBool(&resolutionConfig, "IFACE", "IFACE_BAR_SIDES_ORI", &gInterfaceSidePanelsExtendFromScreenEdge);
			}
			configFree(&resolutionConfig);
		}*/

	if (_GNW95_init_window(width, height, fullscreen, scale) == -1) {
		return -1;
	}

	if (directDrawInit(width, height, bpp) == -1) {
		return -1;
	}

	_scr_size.left = 0;
	_scr_size.top = 0;
	_scr_size.right = width - 1;
	_scr_size.bottom = height - 1;

	_mouse_blit_trans = nullptr;
	_scr_blit = _GNW95_ShowRect;
	_zero_mem = _GNW95_zero_vid_mem;
	_mouse_blit = _GNW95_ShowRect;

	return 0;
}

// 0x4CAECC
int _init_vesa_mode(int width, int height) {
	return _GNW95_init_mode_ex(width, height, 8);
}

// 0x4CAEDC
int _GNW95_init_window(int width, int height, bool fullscreen, int scale) {
//	if (gSdlWindow == nullptr) {
//		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

//		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//			return -1;
//		}

//		Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

//		if (fullscreen) {
//			windowFlags |= SDL_WINDOW_FULLSCREEN;
//		}

//		gSdlWindow = SDL_CreateWindow(gProgramWindowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * scale, height * scale, windowFlags);
//		if (gSdlWindow == nullptr) {
//			return -1;
//		}

//		if (!createRenderer(width, height)) {
//			destroyRenderer();

//			SDL_DestroyWindow(gSdlWindow);
//			gSdlWindow = nullptr;

//			return -1;
//		}
		createRenderer(width, height);
//	}

	return 0;
}

// 0x4CAF9C
int directDrawInit(int width, int height, int bpp) {
	if (gSdlSurface != nullptr) {
		unsigned char *palette = directDrawGetPalette();
		directDrawFree();

		if (directDrawInit(width, height, bpp) == -1) {
			return -1;
		}

		directDrawSetPalette(palette);

		return 0;
	}
	gSdlSurface = new Graphics::Surface();
	gSdlSurface->create(width, height,Graphics::PixelFormat(Graphics::PixelFormat::createFormatCLUT8()));
//	 = SDL_CreateRGBSurface(0, width, height, bpp, 0, 0, 0, 0);

//	SDL_Color colors[256];
	unsigned char new_palette[768];
	for (int index = 0; index < 256; index++) {
		new_palette[index * 3] = index;
		new_palette[index * 3 + 1] = index;
		new_palette[index * 3 + 2] = index;
	}

//	SDL_SetPaletteColors(gSdlSurface->format->palette, colors, 0, 256);
	g_system->getPaletteManager()->setPalette(new_palette, 0, 256);
	g_system->updateScreen(); // TODO is this enough?

	return 0;
}

// 0x4CB1B0
void directDrawFree() {
	if (gSdlSurface != nullptr) {
//		SDL_FreeSurface(gSdlSurface);
		gSdlSurface->free();
		gSdlSurface = nullptr;
	}
}

// 0x4CB310
void directDrawSetPaletteInRange(unsigned char *palette, int start, int count) {
	//	if (gSdlSurface != nullptr && gSdlSurface->format->palette != nullptr) {
	//		SDL_Color colors[256];
	unsigned char new_palette[768];

	/*		if (count != 0) {
				for (int index = 0; index < count; index++) {
					colors[index].r = palette[index * 3] << 2;
					colors[index].g = palette[index * 3 + 1] << 2;
					colors[index].b = palette[index * 3 + 2] << 2;
					colors[index].a = 255;
				}
			}*/

	if (count != 0)
		for (int index = 0; index < 768; index++)
			new_palette[count] = palette[index] << 2;

	g_system->getPaletteManager()->setPalette(new_palette, start, count);
	//		SDL_SetPaletteColors(gSdlSurface->format->palette, colors, start, count);
	//		SDL_BlitSurface(gSdlSurface, nullptr, gSdlTextureSurface, nullptr);
	g_system->updateScreen(); // TODO is this enough?
}
//}

// 0x4CB568
void directDrawSetPalette(unsigned char *palette) {
	//	if (gSdlSurface != nullptr && gSdlSurface->format->palette != nullptr) {
	//		SDL_Color colors[256];
	unsigned char new_palette[768];

	for (int index = 0; index < 768; index++)
		new_palette[index] = palette[index] << 2;

	g_system->getPaletteManager()->setPalette(new_palette, 0, 256);
//	SDL_SetPaletteColors(gSdlSurface->format->palette, colors, 0, 256);
//	SDL_BlitSurface(gSdlSurface, nullptr, gSdlTextureSurface, nullptr);
	g_system->updateScreen();  // TODO is this enough?
}


// 0x4CB68C
unsigned char *directDrawGetPalette() {
	// 0x6ACA24
	static unsigned char palette[768];

	// TODO use surface.grabpalette?

	g_system->getPaletteManager()->grabPalette(palette, 0, 256);

	//	if (gSdlSurface != nullptr && gSdlSurface->format->palette != nullptr) {
	//		SDL_Color *colors = gSdlSurface->format->palette->colors;

	/*		for (int index = 0; index < 256; index++) {
				SDL_Color *color = &(colors[index]);
				palette[index * 3] = color->r >> 2;
				palette[index * 3 + 1] = color->g >> 2;
				palette[index * 3 + 2] = color->b >> 2;
			}*/
	for (int index = 0; index < 768; index++)
		palette[index] = palette[index] >> 2;
	//	}

	return palette;
}

// 0x4CB850
void _GNW95_ShowRect(unsigned char *src, int srcPitch, int a3, int srcX, int srcY, int srcWidth, int srcHeight, int destX, int destY) {
	blitBufferToBuffer(src + srcPitch * srcY + srcX, srcWidth, srcHeight, srcPitch, (unsigned char *)gSdlSurface->getPixels() + gSdlSurface->pitch * destY + destX, gSdlSurface->pitch);

	Common::Rect srcRect;
	srcRect.left = destX;
	srcRect.top = destY;
	srcRect.right = srcRect.left + srcWidth;
	srcRect.bottom = srcRect.top + srcHeight;

	const Common::Point destPos(destX, destY);

	byte palette[256 * 3];
	g_system->getPaletteManager()->grabPalette(palette, 0, 256);

	const Graphics::Surface &newSurf = *gSdlSurface;
	gSdlTextureSurface->blitFrom(newSurf, srcRect, destPos, palette);

	//	SDL_BlitSurface(gSdlSurface, &srcRect, gSdlTextureSurface, &destRect);
}

// Clears drawing surface.
//
// 0x4CBBC8
void _GNW95_zero_vid_mem() {
	if (!gProgramIsActive) {
		return;
	}

	unsigned char *surface = (unsigned char *)gSdlSurface->getPixels();
	for (int y = 0; y < gSdlSurface->h; y++) {
		memset(surface, 0, gSdlSurface->w);
		surface += gSdlSurface->pitch;
	}

//	SDL_BlitSurface(gSdlSurface, nullptr, gSdlTextureSurface, nullptr);
	gSdlTextureSurface->blitFrom(gSdlSurface);
}

int screenGetWidth() {
	// TODO: Make it on par with _xres;
	return rectGetWidth(&_scr_size);
}

int screenGetHeight() {
	// TODO: Make it on par with _yres.
	return rectGetHeight(&_scr_size);
}

int screenGetVisibleHeight() {
	int windowBottomMargin = 0;

	if (!gInterfaceBarMode) {
		windowBottomMargin = INTERFACE_BAR_HEIGHT;
	}
	return screenGetHeight() - windowBottomMargin;
}

static bool createRenderer(int width, int height) {
/*	gSdlRenderer = SDL_CreateRenderer(gSdlWindow, -1, 0);
	if (gSdlRenderer == nullptr) {
		return false;
	}

	if (SDL_RenderSetLogicalSize(gSdlRenderer, width, height) != 0) {
		return false;
	}

	gSdlTexture = SDL_CreateTexture(gSdlRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (gSdlTexture == nullptr) {
		return false;
	}

	Uint32 format;
	if (SDL_QueryTexture(gSdlTexture, &format, nullptr, nullptr, nullptr) != 0) {
		return false;
	}*/

//	gSdlTextureSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, SDL_BITSPERPIXEL(format), format);
	gSdlTextureSurface = new Graphics::ManagedSurface();
	gSdlTextureSurface->create(width, height, Graphics::PixelFormat(Graphics::PixelFormat::createFormatCLUT8()));

	if (gSdlTextureSurface == nullptr) {
		return false;
	}

	return true;
}

static void destroyRenderer() {
/*	if (gSdlTextureSurface != nullptr) {  TODO
		SDL_FreeSurface(gSdlTextureSurface);
		gSdlTextureSurface = nullptr;
	}

	if (gSdlTexture != nullptr) {
		SDL_DestroyTexture(gSdlTexture);
		gSdlTexture = nullptr;
	}

	if (gSdlRenderer != nullptr) {
		SDL_DestroyRenderer(gSdlRenderer);
		gSdlRenderer = nullptr;
	}*/
}

void handleWindowSizeChanged() {
//	destroyRenderer();
//	createRenderer(screenGetWidth(), screenGetHeight());
}

void renderPresent() {
	//	SDL_UpdateTexture(gSdlTexture, nullptr, gSdlTextureSurface->pixels, gSdlTextureSurface->pitch);
	//	SDL_RenderClear(gSdlRenderer);
	//	SDL_RenderCopy(gSdlRenderer, gSdlTexture, nullptr, nullptr);
	//	SDL_RenderPresent(gSdlRenderer);

	const Graphics::ManagedSurface &newScreen = *gSdlTextureSurface;

	g_engine->_screen->copyFrom(newScreen);
	g_engine->_screen->update();
}

} // namespace Fallout2
