#ifndef ISO_ENGINE_RENDERER_H
#define ISO_ENGINE_RENDERER_H

#include <memory>
#include <SDL_render.h>
#include "Display.h"

class Renderer {
public:
    inline static std::string windowName_ = "ISO-Engine";

    bool init(const std::string &name = windowName_,
            // TODO default window size selection - use these numbers - q*bert looks best at 4:3 aspect ratio
              Uint32 windowWidth = 1280,  // Display::WIDTH/2
              Uint32 windowHeight = 960);  // Display::HEIGHT/2
    void shutdown();
    void show() const;  // TODO remove?

    SDL_Rect viewArea() const;
    SDL_Window &getWindow() const;
    SDL_Renderer &getRenderer() const;

    int windowWidth_; // TODO make these static
    int windowHeight_;

private:
    // Default initialize w/nullptr since SDL needs to initialize first
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_{nullptr, SDL_DestroyWindow};
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> internalRenderer_{nullptr, SDL_DestroyRenderer};
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> screenSurface_{nullptr, SDL_FreeSurface};  // temporary


};

#endif //ISO_ENGINE_RENDERER_H
