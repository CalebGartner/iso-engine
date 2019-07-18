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
    void processInput();
    void render(const Renderer &renderer) const;

    inline double getScreenX() const;
    inline double getScreenY() const;

    double x_, y_;
    int width_, height_;
private:
    int dX, dY;
    // TODO make shared_pointer, reset() on update() calls
    PlayerState &state_ = PlayerState::Still;
    std::vector<std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>> playerTextures_;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> playerTexture_{nullptr, SDL_DestroyTexture};
};


// PlayerState: MovingState: StillState: - make static
// TODO give each state update() methods and transfer x_, y_ (dX/dY as well? or just use x_ += ... calculations) over


#endif //ISO_ENGINE_PLAYER_H
