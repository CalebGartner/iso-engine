#ifndef ISO_ENGINE_LEVEL_H
#define ISO_ENGINE_LEVEL_H

#include <string>
#include <forward_list>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "cpptoml.h"
#include "Audio.h"
#include "UILayer.h"
#include "Renderer.h"
#include "EngineUtils.h"
#include "Player.h"
#include "Tile.h"


typedef std::vector<std::vector<std::forward_list<Tile>>>::size_type map_size_type;


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

    static SDL_Point gridOrigin_;
private:
    SDL_Event event_;
    Uint32 levelID_;
    Uint32 bonus_;
    Uint32 minScreenTileHeight_;

    std::vector<std::vector<std::forward_list<Tile>>> map_;

    std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> DeathSound{nullptr, Mix_FreeChunk};
};

#endif //ISO_ENGINE_LEVEL_H
