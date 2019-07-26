#ifndef ISO_ENGINE_UILAYER_H
#define ISO_ENGINE_UILAYER_H

#include "Renderer.h"
#include "SDL_ttf.h"


extern Uint32 ISO_UI_EVENT;

class UILayer {
public:
    static int FontSize;

    UILayer() = default;
    virtual ~UILayer() { shutdown(); };

    bool init(const cpptoml::table &config);
    void render(const Renderer &renderer) const;
    void update(const Renderer &renderer);
    void processInput(const SDL_Event &event);
    void shutdown();

private:
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> score_{nullptr, SDL_DestroyTexture};
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> level_{nullptr, SDL_DestroyTexture};
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> lifeCounter_{nullptr, SDL_DestroyTexture};

    std::string scoreText_ = "0000000";
    std::string levelText_ = "LEVEL: 1";
    std::string lifeCounterText_ = "LIVES: 4";

    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font_{nullptr, TTF_CloseFont};
    SDL_Color textColor_{0, 0, 0, 0};

    SDL_Texture *loadTextTexture(const std::string& text, const Renderer &renderer);
};


#endif //ISO_ENGINE_UILAYER_H
