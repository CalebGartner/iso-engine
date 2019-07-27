#include "EngineUtils.h"
/*
 * A decent chunk of the following code comes from this tutorial:
 *      https://www.willusher.io/sdl2%20tutorials/2014/06/16/postscript-0-properly-finding-resource-paths
 * Project directory structure:
 * bin/
 * lib/
 * cmake/
 *     sdl2/
 * include/
 * src/
 * res/
 *
 */

namespace EngineUtils {
    std::string getResourcePath(std::string subPath) {

#ifdef _WIN32
        const char PATH_SEP = '\\';
#else
        const char PATH_SEP = '/';
#endif

        static std::string resPath;  // static so we only need to find it once . . .

        if (resPath.empty()) {
            char *basePath = SDL_GetBasePath();
            if (basePath) {
                resPath = basePath;
                SDL_free(basePath);
            } else {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                         "ERROR RETRIEVING RESOURCE PATH",
                                         SDL_GetError(),
                                         nullptr);
                return "";  // use 'nullptr' instead?
            }
            // We replace the last bin/ with res/ to get the the resource path
            size_t pos = resPath.rfind("bin");
            resPath = resPath.substr(0, pos) + "res" + PATH_SEP;
        }
        std::replace(subPath.begin(), subPath.end(), '|', PATH_SEP);
        // If we want a specific subdirectory path in the resource directory
        return subPath.empty() ? resPath : resPath + subPath;
    }

    SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &resource) {
        auto surface = IMG_Load(EngineUtils::getResourcePath(resource).c_str());
        if (surface == nullptr) {
            printf("Unable to load image %s! SDL_image Error: %s\n", resource.c_str(), IMG_GetError());
            return nullptr;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }
}
