#ifndef ISO_ENGINE_AUDIO_H
#define ISO_ENGINE_AUDIO_H

#include "SDL_audio.h"
#include "SDL_mixer.h"

namespace Audio {

    extern bool init();

    // TODO AUDIO - begin background music - use SDL_PeepEvents() to check for audio events - it's thread safe
    // TODO rename to run() ?
    extern int start(void* data);  // thread function . . .

}


#endif //ISO_ENGINE_AUDIO_H
