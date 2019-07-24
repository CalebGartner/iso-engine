#ifndef ISO_ENGINE_AUDIO_H
#define ISO_ENGINE_AUDIO_H

#include "SDL_mixer.h"
#include "EngineUtils.h"

namespace Audio {
    extern bool init();
    extern void shutdown();
    extern Mix_Chunk *loadMixChunk(std::string audioFile);
    extern Mix_Music *loadMixMusic(std::string audioFile);
}


#endif //ISO_ENGINE_AUDIO_H
