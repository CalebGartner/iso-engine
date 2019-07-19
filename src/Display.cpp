#include "Display.h"

namespace Display {
    SDL_DisplayMode current;
    Uint32 MS_PER_UPDATE = 0;  // i.e., SKIP_TICK/INTERVAL - filled by init()
    Uint32 REFRESH_RATE = 0;  // i.e., TICKS_PER_SECOND - filled by init()
    Uint32 WIDTH = 0;
    Uint32 HEIGHT = 0;
    Uint32 PIXEL_FORMAT = 0;

    // Called by Game::init() along w/other engine components
    bool init() {
        if (SDL_GetCurrentDisplayMode(0, &Display::current) != 0) return false;

        REFRESH_RATE = current.refresh_rate;
        MS_PER_UPDATE = 1000 / REFRESH_RATE;
        WIDTH = current.w;
        HEIGHT = current.h;
        PIXEL_FORMAT = current.format;  // enum

        return true;
    }
}
