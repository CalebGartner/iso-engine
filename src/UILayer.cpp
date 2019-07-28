#include "UILayer.h"


int EngineUI::Score = 0;
int EngineUI::Level = 1;
int EngineUI::Lives = 4;
int UILayer::FontSize = 78;


bool UILayer::init(const cpptoml::table &config) {
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
    levelText_ = "LEVEL: " + std::to_string(EngineUI::Level);
    level_.reset(loadTextTexture(levelText_, renderer));

    scoreText_ = std::to_string(EngineUI::Score);
    for (int i = 7; i != (int)scoreText_.length();) {
        scoreText_.append("0");
    }
    score_.reset(loadTextTexture(scoreText_, renderer));

    lifeCounterText_ = "LIVES: " + std::to_string(EngineUI::Lives);
    lifeCounter_.reset(loadTextTexture(lifeCounterText_, renderer));
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
