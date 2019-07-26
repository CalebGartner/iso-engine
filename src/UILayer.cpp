#include "UILayer.h"

Uint32 ISO_UI_EVENT = SDL_RegisterEvents(1);
int UILayer::FontSize = 78;

bool UILayer::init(const cpptoml::table &config) {
    if (ISO_UI_EVENT == ((Uint32) - 1)) return false;

    if (TTF_Init() == -1) {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return false;
    }

    std::string font_file = EngineUtils::getResourcePath(*config.get_as<std::string>("uifont"));
    font_.reset(TTF_OpenFont(font_file.c_str(), UILayer::FontSize));
    if (!font_) {
        printf( "Failed to load font %s! SDL_ttf Error: %s\n", font_file.c_str(), TTF_GetError() );
        return false;
    }

    return true;
}

void UILayer::render(const Renderer &renderer) const {
    int w, h;

    TTF_SizeText(font_.get(), lifeCounterText_.c_str(), &w, &h);
    SDL_Rect rect = {25, (Renderer::WindowHeight / 10), w, h};
    SDL_RenderCopy(&renderer.getRenderer(), lifeCounter_.get(), nullptr, &rect);

    TTF_SizeText(font_.get(), scoreText_.c_str(), &w, &h);
    rect = {25, ((Renderer::WindowHeight / 10) - h), w, h};
    SDL_RenderCopy(&renderer.getRenderer(), score_.get(), nullptr, &rect);

    TTF_SizeText(font_.get(), levelText_.c_str(), &w, &h);
    rect = {(Renderer::WindowWidth - w - 25), (Renderer::WindowHeight / 10), w, h};
    SDL_RenderCopy(&renderer.getRenderer(), level_.get(), nullptr, &rect);
}

void UILayer::update(const Renderer &renderer) {
    // Update textures
    level_.reset(loadTextTexture(levelText_, renderer));
    score_.reset(loadTextTexture(scoreText_, renderer));
    lifeCounter_.reset(loadTextTexture(lifeCounterText_, renderer));
}

void UILayer::processInput(const SDL_Event &event) {
    // Change texture text
    switch (event.user.code) {
        case 0:  // TODO extern case ints as variables
            levelText_ = "LEVEL: " + std::to_string(*(Uint32*)event.user.data1);
            break;
        case 1:
            scoreText_ = std::to_string(std::strtol(scoreText_.c_str(), nullptr, 0) + *(Uint32*)event.user.data1);
            for (int i = 7; i != (int)scoreText_.length();) {
                scoreText_.append("0");
            }
            break;
        case 2:
            lifeCounterText_ = "LIVES: " + std::to_string(std::strtol(lifeCounterText_.c_str(), nullptr, 0) + *(int*)event.user.data1);
            break;
        default:
            break;
    }
}

void UILayer::shutdown() {
    level_.reset();
    score_.reset();
    lifeCounter_.reset();
    font_.reset();
    TTF_Quit();
}

SDL_Texture *UILayer::loadTextTexture(const std::string &text, const Renderer &renderer) {
    SDL_Surface *temp = TTF_RenderText_Solid(font_.get(), text.c_str(), textColor_);
    if (temp == nullptr) {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    } else {
        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(&renderer.getRenderer(), temp);
        SDL_FreeSurface(temp);
        return text_texture;
    }
}
