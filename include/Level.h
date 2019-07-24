#ifndef ISO_ENGINE_LEVEL_H
#define ISO_ENGINE_LEVEL_H

#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "cpptoml.h"
#include "Audio.h"
#include "Renderer.h"
#include "EngineUtils.h"
#include "Player.h"

extern int TILE_WIDTH_HALF;
extern int TILE_HEIGHT_HALF;
extern double TILE_HEIGHT_WIDTH_RATIO;
extern Uint32 ISO_TILE_TOUCHED;


class Level {
public:
    explicit Level(Uint32 levelID)
    : levelID_(levelID)
    {};

    bool init(const Renderer &renderer, const cpptoml::table &config);
    void render(const Renderer &renderer) const;
    void update();
    void processInput(const SDL_Event &event);
    void shutdown();

    // These offsets essentially become the new tile grid origin - defaults to [0,0]
    int xOffset_ = 0, yOffset_ = 0;

private:
    SDL_UserEvent event_;
    Uint32 levelID_;
    Uint32 scorePerTile_ = 30;
    Uint32 bonus_ = 1000;
    Uint32 minScreenTileHeight_ = 6;

    // TODO typedef any of the below??
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tileTouched_{nullptr, SDL_DestroyTexture};
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tileUntouched_{nullptr, SDL_DestroyTexture};
    // TODO make each node a linked-list of Tiles instead . . . make the player a subclass of Tile . . give each an update method
    std::vector<std::vector<SDL_Texture*>> map_;  // TODO make public?

    std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> tileTouchedSound_{nullptr, Mix_FreeChunk};
    std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> tileUntouchedSound_{nullptr, Mix_FreeChunk};
    std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> offMapSound_{nullptr, Mix_FreeChunk};

    SDL_Texture *loadTexture(const Renderer &renderer, const std::string &resource);
};

#endif //ISO_ENGINE_LEVEL_H
