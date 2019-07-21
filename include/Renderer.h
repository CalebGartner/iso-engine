#ifndef ISO_ENGINE_RENDERER_H
#define ISO_ENGINE_RENDERER_H

#include <memory>
#include "SDL_render.h"
#include "SDL_image.h"
#include "Display.h"



class Renderer {
public:
    inline static int WindowWidth = 1280;
    inline static int WindowHeight = 960;
    inline static std::string WindowName = "ISO-Engine";

    bool init(const std::string &name = WindowName,
              Uint32 windowWidth = WindowWidth,
              Uint32 windowHeight = WindowHeight);
    void shutdown();
    void show() const;

    SDL_Rect viewArea() const;
    SDL_Window &getWindow() const;
    SDL_Renderer &getRenderer() const;

private:
    // Default initialize w/nullptr since SDL needs to initialize first
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_{nullptr, SDL_DestroyWindow};
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> internalRenderer_{nullptr, SDL_DestroyRenderer};
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> screenSurface_{nullptr, SDL_FreeSurface};  // temporary
};

#endif //ISO_ENGINE_RENDERER_H
