#ifndef ISO_ENGINE_RENDERER_H
#define ISO_ENGINE_RENDERER_H

#include <SDL.h>

class Renderer {
public:

    bool                        Init(const char* name = "Iso-Engine", int windowWidth = 1500, int windowHeight = 800);
    void                        Shutdown() const;
    void                        Show() const;

    SDL_Rect                    ViewArea() const;
    SDL_Window*                 GetWindow() const;
    SDL_Renderer*               GetRenderer() const;

private:

    SDL_Window*                 window;
    SDL_Renderer*               internal_renderer;
};


#endif //ISO_ENGINE_RENDERER_H
