#include <utility>

#include "Audio.h"

namespace Audio {
    static int CHANNELS = 2;
    static int FREQUENCY = 22050;
    static int CHUNK_SIZE = 2048;

    bool init() {
        if (Mix_OpenAudio(FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS, CHUNK_SIZE) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            return false;
        }
        Mix_Init(MIX_INIT_MP3);

        return true;
    }

    void shutdown() {
        Mix_CloseAudio();
        Mix_Quit();
    }

    Mix_Chunk *loadMixChunk(std::string audioFile) {
        std::string effect_file = EngineUtils::getResourcePath(std::move(audioFile));
        Mix_Chunk *effect = Mix_LoadWAV(effect_file.c_str());
        if (effect == nullptr) {
            printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        }
        return effect;
    }

    Mix_Music *loadMixMusic(std::string audioFile) {
        std::string music_file = EngineUtils::getResourcePath(std::move(audioFile));
        Mix_Music *music = Mix_LoadMUS(music_file.c_str());
        if (music == nullptr) {
            printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        }
        return music;
    }
}
