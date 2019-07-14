#include <Renderer.h>
#include <SDL_image.h>
#include "PathUtils.h"
#include "Level.h"

bool Level::init(const Renderer &renderer) {
    std::shared_ptr<cpptoml::table> config;
    try {
        // TODO make q*bert.toml a var somewhere
        config = cpptoml::parse_file(PathUtils::getResourcePath() + "q*bert.toml");
    } catch (const cpptoml::parse_exception &e) {
        SDL_Log("PARSE ERROR: %s", e.what());
        return false;
    }

    auto anchor = *(config->get_array_of<int64_t >("topcenteranchor"));
    std::copy(anchor.begin(), anchor.end(), topCenterAnchor_);
    scorePerTile_ = *(config->get_as<Uint32>("scorepertile"));
    bonus_ = *(config->get_as<Uint32>("bonus"));

    auto tarr = config->get_table_array("level");
    std::shared_ptr<cpptoml::table> level;
    for (const auto &table : *tarr) {
        // TODO change this to an inner table instead - use get_table_qualified("table.inner")
        auto id = table->get_as<Uint32>("id");
        if (*id == levelID_) {
            level = table;
            break;
        }
    }

    auto inner = level->get_table("tiles");

    // TODO make below into a function - separate init() into other functions - check for null/error when loading/creating texture/img
    std::string touched = *(inner->get_as<std::string>("touched"));
    auto touchedSurface = IMG_Load(touched.c_str());
    tileTouched_.reset(SDL_CreateTextureFromSurface(&renderer.getRenderer(),
                                                    touchedSurface));
    SDL_FreeSurface(touchedSurface);

    std::string untouched = *(inner->get_as<std::string>("untouched"));
    auto untouchedSurface = IMG_Load(untouched.c_str());
    tileUntouched_.reset(SDL_CreateTextureFromSurface(&renderer.getRenderer(),
                                                      untouchedSurface));
    SDL_FreeSurface(untouchedSurface);

//    if( loadedSurface == NULL )
//    {
//        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
//    }

    minScreenTileWidth_ = *(level->get_as<Uint32>("minscreentilewidth"));
    minScreenTileHeight_ = *(level->get_as<Uint32>("minscreentileheight"));

    auto levelMap = level->get_array_of<cpptoml::array>("map");
    map_ = std::vector<std::vector<SDL_Texture*>>(levelMap->size());
    int i = 0;

    for (const auto &arr: *levelMap) {
        const auto line = arr->get_array_of<int64_t>();
        map_[i] = std::vector<SDL_Texture*>(line->size());
        for (const auto &tileType: *line) {
            auto it = map_[i].begin();
            if (tileType == 0) map_[i].insert(it, nullptr);
            if (tileType == 1) map_[i].insert(it, tileUntouched_.get());
            if (tileType == 2) map_[i].insert(it, nullptr);  // TODO add disc sprite
            if (tileType == 3) map_[i].insert(it, tileTouched_.get());
        }
        ++i;
    }

    return true;
}
