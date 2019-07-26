#include "Renderer.h"

#define WINDOW_FLAGS (SDL_WINDOW_SHOWN)
// | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_FULLSCREEN_DESKTOP)
#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC)
#define IMG_FLAGS (IMG_INIT_PNG | IMG_INIT_JPG)


bool Renderer::init(const cpptoml::table &config, const std::string &name, Uint32 windowWidth, Uint32 windowHeight) {
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

    screenSurface_.reset(SDL_GetWindowSurface(window_.get()));

    if (!IMG_Init(IMG_FLAGS)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    std::string background_file = EngineUtils::getResourcePath("background.jpg");
    SDL_Surface *temp = IMG_Load(background_file.c_str());
    if (!temp) {
        printf("Unable to load image %s! SDL_image Error: %s\n", background_file.c_str(), IMG_GetError());
        return false;
    }

    background_.reset(SDL_CreateTextureFromSurface(internalRenderer_.get(), temp));
    SDL_FreeSurface(temp);

    return true;
}

void Renderer::shutdown() {
    screenSurface_.reset();
    internalRenderer_.reset();
    window_.reset();
    IMG_Quit();
}

void Renderer::show() const {
    SDL_RenderCopy(internalRenderer_.get(), background_.get(), nullptr, nullptr);
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
