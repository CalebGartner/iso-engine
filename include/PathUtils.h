#ifndef ISO_ENGINE_PATHUTILS_H
#define ISO_ENGINE_PATHUTILS_H

#include <string>
#include <iostream>
#include <SDL.h>

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

namespace PathUtils {
    std::string getResourcePath(const std::string &subDir = "") {

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
        // If we want a specific subdirectory path in the resource directory
        return subDir.empty() ? resPath : resPath + subDir + PATH_SEP;
    }
}


#endif //ISO_ENGINE_PATHUTILS_H
