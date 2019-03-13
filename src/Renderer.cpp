#include <SDL.h>
#include "Renderer.h"
#include "PathUtils.h"

int Renderer::screenWidth = 1500;
int Renderer::screenHeight = 800;

bool Renderer::Init(const char* name, int windowWidth, int windowHeight) {
    window = SDL_CreateWindow( name,
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               windowWidth,
                               windowHeight,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

    if (!window)
        return false;

    // DEBUG: SDL_RENDERER_TARGETTEXTURE allows rendering to SDL_Textures
    internal_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    if (!internal_renderer)
        return false;

    SDL_Rect viewArea;
    SDL_RenderGetViewport(internal_renderer, &viewArea);  // implement later . . .
    // TODO create overlay vector of render images that remain static regardless of screen content - e.g., UI overlay

    // Get window surface - temporary
    screenSurface = SDL_GetWindowSurface(window);

    return true;
}

void Renderer::Shutdown() const {
    SDL_FreeSurface(screenSurface);

    if (internal_renderer)
        SDL_DestroyRenderer(internal_renderer);

    if (window)
        SDL_DestroyWindow(window);
}

void Renderer::Show() const {
//    SDL_RenderPresent(internal_renderer);

    // Fill the surface white
     SDL_FillRect( screenSurface, nullptr, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
    // Update the surface
     SDL_UpdateWindowSurface(window);
    // Wait two seconds
     SDL_Delay( 500 );
}

SDL_Rect Renderer::ViewArea() const {
    SDL_Rect viewArea;
    SDL_RenderGetViewport(internal_renderer, &viewArea);  // init
    return viewArea;
}

SDL_Window* Renderer::GetWindow() const {
    return window;
}

SDL_Renderer* Renderer::GetRenderer() const {
    return internal_renderer;
}
