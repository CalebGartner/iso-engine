#ifndef ISO_ENGINE_RENDERER_H
#define ISO_ENGINE_RENDERER_H

#include <memory>
#include "SDL_render.h"
#include "Display.h"

class Renderer {
public:
    bool init(const char *name = "ISO-Engine",
              Uint32 windowWidth = Display::WIDTH,
              Uint32 windowHeight = Display::HEIGHT);
    void shutdown();
    void show() const;

    SDL_Rect viewArea() const;
    std::shared_ptr<SDL_Window> getWindow() const;
    std::shared_ptr<SDL_Renderer> getRenderer() const;

private:
    // Default initialize w/nullptr since SDL needs to initialize first
    std::shared_ptr<SDL_Window> window_{nullptr, SDL_DestroyWindow};
    std::shared_ptr<SDL_Renderer> internalRenderer_{nullptr, SDL_DestroyRenderer};
    std::shared_ptr<SDL_Surface> screenSurface_{nullptr, SDL_FreeSurface};  // temporary

};

#endif //ISO_ENGINE_RENDERER_H
