#ifndef ISO_ENGINE_DISPLAY_H
#define ISO_ENGINE_DISPLAY_H

#include <SDL_video.h>

namespace Display {
    extern SDL_DisplayMode current;
    extern Uint32 MS_PER_UPDATE;  // i.e., SKIP_TICK/INTERVAL - filled by init()
    extern Uint32 REFRESH_RATE;  // i.e., TICKS_PER_SECOND - filled by init()
    extern Uint32 WIDTH;
    extern Uint32 HEIGHT;
    extern Uint32 PIXEL_FORMAT;

    // Called by Game::init() along w/other engine components
    extern bool init();

}


#endif //ISO_ENGINE_DISPLAY_H
