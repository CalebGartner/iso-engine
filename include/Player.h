#ifndef ISO_ENGINE_PLAYER_H
#define ISO_ENGINE_PLAYER_H

#include <memory>
#include <SDL_render.h>
#include "Renderer.h"

class Player {
public:
    void update();
    void render(const Renderer &renderer) const;

private:
    int x_, y_;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> playerTexture_{nullptr, SDL_DestroyTexture};
};

#endif //ISO_ENGINE_PLAYER_H
