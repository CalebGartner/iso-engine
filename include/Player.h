#ifndef ISO_ENGINE_PLAYER_H
#define ISO_ENGINE_PLAYER_H

#include <memory>
#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_log.h>
#include "cpptoml.h"
#include "Renderer.h"
#include "Level.h"
#include "PlayerState.h"

static double PLAYER_TILE_RATIO = 0.75;


class Player {
public:
    bool init(const Renderer &renderer, const cpptoml::table &config);
    void processInput(const SDL_Event &event);
    void render(const Renderer &renderer) const;
    void update();

    inline double getScreenX() const;
    inline double getScreenY() const;

    static SDL_Rect rect_;  // screen position
    static PlayerState *state_;  // TODO state push-down automata
    double x_, y_;  // grid position
    int dX_, dY_;
private:
    // TODO add continuous player animation
    std::vector<std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>> playerTextures_;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> playerTexture_{nullptr, SDL_DestroyTexture};
};

#endif //ISO_ENGINE_PLAYER_H
