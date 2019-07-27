#ifndef ISO_ENGINE_ENGINEUTILS_H
#define ISO_ENGINE_ENGINEUTILS_H

#include <string>
#include <iostream>
#include <algorithm>
#include "SDL.h"
#include "SDL_image.h"

namespace EngineUtils {
    std::string getResourcePath(std::string subPath = "");
    SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &resource);
}

#endif //ISO_ENGINE_ENGINEUTILS_H
