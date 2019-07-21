#include "Audio.h"

namespace Audio {

    bool init() {
        // TODO fix magic numbers
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            return false;
        }
        return true;
    }

    int start(void *data) {
        return 0;
    }
}
