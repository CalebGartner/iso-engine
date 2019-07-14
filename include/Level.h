#ifndef ISO_ENGINE_LEVEL_H
#define ISO_ENGINE_LEVEL_H

#include <string>
#include "cpptoml.h"
#include "SDL.h"

// TODO double buffer the map?
// TODO make Tile struct w/SDL_Rect to render on for each tile/other vars?

class Level {
public:
    explicit Level(Uint32 levelID)
    : levelID_(levelID)
    {};

    bool init(const Renderer& renderer);

    static std::string gameConfig_;

private:
    Uint32 levelID_;
    Uint32 scorePerTile_ = 30;
    Uint32 bonus_ = 1000;
    Uint32 minScreenTileWidth_ = 10;
    Uint32 minScreenTileHeight_ = 10;

    // TODO typedef any of the below??
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tileTouched_{nullptr, SDL_DestroyTexture};
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tileUntouched_{nullptr, SDL_DestroyTexture};

    std::vector<std::vector<SDL_Texture*>> map_;  // TODO make public?
    int topCenterAnchor_[2] = {0,0};  // defaults to [0,0]
};

#endif //ISO_ENGINE_LEVEL_H
