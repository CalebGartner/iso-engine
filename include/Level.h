#ifndef ISO_ENGINE_LEVEL_H
#define ISO_ENGINE_LEVEL_H

#include <string>
#include "SDL.h"
#include "cpptoml.h"

// TODO double buffer the map?
static int TILE_WIDTH_HALF;
static int TILE_HEIGHT_HALF;
static double TILE_HEIGHT_WIDTH_RATIO;

class Level {
public:
    inline static std::string GameConfig = "q*bert.toml";  // TODO make Game class variable?

    explicit Level(Uint32 levelID)
    : levelID_(levelID)
    {};

    bool init(const Renderer &renderer);
    void render(const Renderer &renderer) const;
    void renderTile(SDL_Texture *texture, int x, int y, SDL_Rect *clip = nullptr, double angle = 0.0, SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    // These offsets essentially become the new tile grid origin - defaults to [0,0]
    int xOffset_ = 0, yOffset_ = 0;

private:
    Uint32 levelID_;
    Uint32 scorePerTile_ = 30;
    Uint32 bonus_ = 1000;
    Uint32 minScreenTileHeight_ = 6;

    // TODO typedef any of the below??
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tileTouched_{nullptr, SDL_DestroyTexture};
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tileUntouched_{nullptr, SDL_DestroyTexture};

    // TODO make each node a linked-list of Tiles instead . . .
    std::vector<std::vector<SDL_Texture*>> map_;  // TODO make public?

    SDL_Texture *loadTexture(const Renderer &renderer, const std::string &resource);
};

#endif //ISO_ENGINE_LEVEL_H
