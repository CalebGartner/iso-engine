#include "Renderer.h"

#define WINDOW_FLAGS (SDL_WINDOW_SHOWN)
// | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_FULLSCREEN_DESKTOP)

#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC)


bool Renderer::init(const std::string &name, Uint32 windowWidth, Uint32 windowHeight) {
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

    // TODO create overlay viewports that remain static regardless of screen content - e.g., UI overlay

    // Get window surface - temporary
    screenSurface_.reset(SDL_GetWindowSurface(window_.get()));

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void Renderer::shutdown() {
    screenSurface_.reset();
    internalRenderer_.reset();
    window_.reset();
    IMG_Quit();
}

void Renderer::show() const {
    // TODO replace w/background blit image
    SDL_SetRenderDrawColor(internalRenderer_.get(), 0x00, 0x00, 0x00, 0x00);
    auto v = viewArea();
    SDL_RenderFillRect(internalRenderer_.get(), &v);
    SDL_SetRenderDrawColor(internalRenderer_.get(), 0xFF, 0xFF, 0xFF, 0xFF);
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
