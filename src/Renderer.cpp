#include "Renderer.h"

#define WINDOW_FLAGS (SDL_WINDOW_SHOWN)
// | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_FULLSCREEN_DESKTOP)

#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC)


bool Renderer::init(const std::string name, Uint32 windowWidth, Uint32 windowHeight) {
    window_.reset(SDL_CreateWindow(name.c_str(),
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   windowWidth,
                                   windowHeight,
                                   WINDOW_FLAGS));

    if (!window_)
        return false;

    // DEBUG: SDL_RENDERER_TARGETTEXTURE allows rendering to SDL_Textures
    internalRenderer_.reset(SDL_CreateRenderer(window_.get(), -1, RENDERER_FLAGS));

    if (!internalRenderer_)
        return false;

    SDL_Rect camera;  // Make into private variable? create separate class?
    SDL_RenderGetViewport(internalRenderer_.get(), &camera);  // implement later . . .
    // TODO create overlay viewports that remain static regardless of screen content - e.g., UI overlay

    // Get window surface - temporary
    screenSurface_.reset(SDL_GetWindowSurface(window_.get()));

    return true;
}

void Renderer::shutdown() {
    // TODO this shouldn't be necessary since they'll automatically delete when the Renderer loses scope
    screenSurface_.reset();
    internalRenderer_.reset();
    window_.reset();
}

void Renderer::show() const {
//    SDL_SetRenderDrawColor(internalRenderer_, 0xFF, 0xFF, 0xFF, 0xFF);
//    SDL_RenderClear(internalRenderer_);
//    // TODO
//    SDL_RenderPresent(internalRenderer_);

    // Fill the surface white
    // TODO replace w/background blit image
    SDL_FillRect(screenSurface_.get(), nullptr, SDL_MapRGB(screenSurface_->format, 0xFF, 0xFF, 0xFF));

    // update the surface
    SDL_UpdateWindowSurface(window_.get());
}

SDL_Rect Renderer::viewArea() const {  // pass in Rect instead? Default arg?
    SDL_Rect viewArea;
    SDL_RenderGetViewport(internalRenderer_.get(), &viewArea);  // init
    return viewArea;
}

SDL_Window &Renderer::getWindow() const {
    return *window_;
}

SDL_Renderer &Renderer::getRenderer() const {
    return *internalRenderer_;
}
