#ifndef ISO_ENGINE_TILE_H
#define ISO_ENGINE_TILE_H

#include <memory>
#include "cpptoml.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "Renderer.h"
#include "Audio.h"

extern int TILE_WIDTH;
extern int TILE_HEIGHT;
extern float TILE_VERTICAL_STAGGER;  // Necessary since the tiles are hexagonal
extern Uint32 ISO_TILE_TOUCHED;


class Tile {
public:
    // Textures
    inline static std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> Touched{nullptr, SDL_DestroyTexture};
    inline static std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> Untouched{nullptr, SDL_DestroyTexture};
    // Audio
    inline static std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> TouchedSound{nullptr, Mix_FreeChunk};
    inline static std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> UntouchedSound{nullptr, Mix_FreeChunk};

    static bool init(const Renderer &renderer, const cpptoml::table &config);
    static void shutdown();

    Tile(SDL_Point point, SDL_Texture *texture)
    : gridPosition_(point), texture_(texture)
    {};
    virtual ~Tile();

    void render(const Renderer &renderer) const;
    void update();
    void processInput(const SDL_Event &event);

    SDL_Point gridPosition_;
    SDL_Rect tileRect_{0, 0, TILE_WIDTH, TILE_HEIGHT};

    SDL_Texture *texture_;
};

#endif //ISO_ENGINE_TILE_H
