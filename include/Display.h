#ifndef ISO_ENGINE_DISPLAY_H
#define ISO_ENGINE_DISPLAY_H

#include <SDL_video.h>

// TODO make 'extern'
namespace Display {
    static SDL_DisplayMode current;
    static Uint32 MS_PER_UPDATE = 0;  // i.e., SKIP_TICK/INTERVAL - filled by init()
    static Uint32 REFRESH_RATE = 0;  // i.e., TICKS_PER_SECOND - filled by init()
    static Uint32 WIDTH = 0;
    static Uint32 HEIGHT = 0;
    static Uint32 PIXEL_FORMAT = 0;

    // Called by Game::init() along w/other engine components
    static bool init() {
        if (SDL_GetCurrentDisplayMode(0, &Display::current) != 0) return false;

        REFRESH_RATE = current.refresh_rate;
        MS_PER_UPDATE = 1000 / REFRESH_RATE;
        WIDTH = current.w;
        HEIGHT = current.h;
        PIXEL_FORMAT = current.format;  // enum

        return true;
    }
}


#endif //ISO_ENGINE_DISPLAY_H
