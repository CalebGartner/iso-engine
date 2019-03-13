#ifndef ISO_ENGINE_RENDERER_H
#define ISO_ENGINE_RENDERER_H

#include <SDL.h>

class Renderer {
public:

    // Screen dimension constants
    static int screenWidth;
    static int screenHeight;

    bool                        Init(const char* name = "Iso-Engine", int windowWidth = screenWidth, int windowHeight = screenHeight);
    void                        Shutdown() const;
    void                        Show() const;

    SDL_Rect                    ViewArea() const;
    SDL_Window*                 GetWindow() const;
    SDL_Renderer*               GetRenderer() const;

private:

    SDL_Window*                 window = nullptr;
    SDL_Renderer*               internal_renderer = nullptr;

    SDL_Surface* screenSurface = nullptr;  // temporary
};

#endif //ISO_ENGINE_RENDERER_H
