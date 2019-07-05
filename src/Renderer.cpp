#include "Renderer.h"
#include "PathUtils.h"


bool Renderer::init(const char *name, Uint32 windowWidth, Uint32 windowHeight) {
    window_.reset(SDL_CreateWindow(name,
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   windowWidth,
                                   windowHeight,
                                   WINDOW_FLAGS), SDL_DestroyWindow);

    if (!window_)
        return false;

    // DEBUG: SDL_RENDERER_TARGETTEXTURE allows rendering to SDL_Textures
    internalRenderer_.reset(SDL_CreateRenderer(window_.get(), -1, RENDERER_FLAGS), SDL_DestroyRenderer);

    if (!internalRenderer_)
        return false;

    SDL_Rect camera;  // Make into private variable? create separate class?
    SDL_RenderGetViewport(internalRenderer_.get(), &camera);  // implement later . . .
    // TODO create overlay viewports that remain static regardless of screen content - e.g., UI overlay

    // Get window surface - temporary
    screenSurface_.reset(SDL_GetWindowSurface(window_.get()), SDL_FreeSurface);

    return true;
}

void Renderer::shutdown() {
    // TODO make sure they're all taken care of . . . change to unique_ptr?
    if (screenSurface_.unique()) screenSurface_.reset();
    if (internalRenderer_.unique()) internalRenderer_.reset();
    if (window_.unique()) window_.reset();
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

std::shared_ptr<SDL_Window> Renderer::getWindow() const {
    return window_;
}

std::shared_ptr<SDL_Renderer> Renderer::getRenderer() const {
    return internalRenderer_;
}
