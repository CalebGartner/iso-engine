#include <Renderer.h>
#include <SDL_image.h>
#include "PathUtils.h"
#include "Level.h"

bool Level::init(const Renderer &renderer) {
    std::shared_ptr<cpptoml::table> config;
    try {
        config = cpptoml::parse_file(PathUtils::getResourcePath(Level::gameConfig_));
    } catch (const cpptoml::parse_exception &e) {
        SDL_Log("PARSE ERROR: %s", e.what());
        return false;
    }

    auto anchor = *(config->get_array_of<int64_t >("origin"));
    std::copy(anchor.begin(), anchor.end(), topCenterAnchor_);
    scorePerTile_ = *(config->get_as<Uint32>("scorepertile"));
    bonus_ = *(config->get_as<Uint32>("bonus"));

    // Get the corresponding level table from the TOML file
    auto tarr = config->get_table_array("level");
    std::shared_ptr<cpptoml::table> level;
    for (const auto &table : *tarr) {
        auto id = table->get_as<Uint32>("id");
        if (*id == levelID_) {
            level = table;
            break;
        }
    }

    // Load the specified tile textures
    std::string touched = *(config->get_qualified_as<std::string>("tiles.touched"));
    tileTouched_.reset(loadTexture(renderer, touched));
    if (!tileTouched_) return false;

    std::string untouched = *(config->get_qualified_as<std::string>("tiles.untouched"));
    tileUntouched_.reset(loadTexture(renderer, untouched));
    if (!tileUntouched_) return false;

    minScreenTileHeight_ = *(level->get_as<Uint32>("minscreentileheight"));

    int windowTileHeight = renderer.windowHeight_ / minScreenTileHeight_;

    int w, h;
    SDL_QueryTexture(tileUntouched_.get(), nullptr, nullptr, &w, &h);
    int tileHeight = windowTileHeight;
    int tileWidth = (w * tileHeight) / h;

    TILE_WIDTH_HALF = tileWidth / 2;
    TILE_HEIGHT_HALF = tileHeight / 2;
    TILE_HEIGHT_WIDTH_RATIO = (static_cast<double>(h)/w) - 1;

    // Load the map
    auto levelMap = level->get_array_of<cpptoml::array>("map");
    map_ = std::vector<std::vector<SDL_Texture*>>(levelMap->size());

    int i = 0;
    for (const auto &arr: *levelMap) {
        const auto line = arr->get_array_of<int64_t>();
        map_[i] = std::vector<SDL_Texture*>(line->size());
        for (const auto &tileType: *line) {
            // TODO change to switch cases
            auto it = map_[i].begin();
            if (tileType == 0) map_[i].insert(it, nullptr);
            if (tileType == 1) map_[i].insert(it, tileUntouched_.get());
            if (tileType == 2) map_[i].insert(it, nullptr);  // TODO add disc sprite
            if (tileType == 3) map_[i].insert(it, tileTouched_.get());
        }
        map_[i].shrink_to_fit();  // Currently the map doesn't change size during runtime . . .
        ++i;
    }
    return true;
}

SDL_Texture *Level::loadTexture(const Renderer &renderer, const std::string &resource) {
    // TODO load from vector .svg sprite sheet instead and optimize the texture/image via SDL_Optimize~~~?
    auto surface = IMG_Load(PathUtils::getResourcePath(resource).c_str());
    if (surface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", resource.c_str(), IMG_GetError());
        return nullptr;
    }
    auto texture = SDL_CreateTextureFromSurface(&renderer.getRenderer(), surface);
    SDL_FreeSurface(surface);
    return texture;
}


void Level::renderTile(SDL_Texture *texture, int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) {
    // TODO change to renderMap() instead? need camera/viewport dimensions?
//    SDL_Rect renderQuad = {x, y, width, height};
//
//    //Set clip rendering dimensions if they exist
//    if(clip != nullptr) {
//        renderQuad.w = clip->w;
//        renderQuad.h = clip->h;
//    }
//
//    // Fortunately, the images used are already rotated/projected isometrically, so there's no need to rotate here
//    SDL_RenderCopyEx(renderer_, &texture, clip, &renderQuad, angle, center, flip);
}

// Sometimes though you have to convert screen pixels back to map coordinates. Example: the player clicks on a pixel;
// how do we reverse the formula and find the tile?
// So final actual commands are:
//map.x = (screen.x / TILE_WIDTH_HALF + screen.y / TILE_HEIGHT_HALF) /2;
//map.y = (screen.y / TILE_HEIGHT_HALF -(screen.x / TILE_WIDTH_HALF)) /2;

void Level::render(const Renderer &renderer) const {
    for(std::vector<std::vector<SDL_Texture*>>::size_type i = 0; i != map_.size(); i++) {
        for(std::vector<SDL_Texture*>::size_type k = 0; k != map_[i].size(); k++) {
            if (map_[i][k] == nullptr) continue;
            // TODO rename to member vars xOffset_, yOffset_ - change these when the player moves to keep the 'camera' centered on the PC
            // TODO create functions for mapToScreen and screenToMap
            int screenX = ((i - topCenterAnchor_[0]) - (k - topCenterAnchor_[1])) * (TILE_WIDTH_HALF);
            // *1.5 is necessary since each tile extends "downwards" - i.e., each one is a hexagonal tile, not a simple diamond
            int screenY = ((i - topCenterAnchor_[0]) + (k - topCenterAnchor_[1])) * (TILE_HEIGHT_HALF*1.5);
            screenX += (renderer.windowWidth_ / 2) - TILE_WIDTH_HALF;  // TODO use viewArea/camera/viewport instead? make static . . .
            // Offsetting the y values by this amount helps correct the map when the tile height and width differ
            screenY -= TILE_HEIGHT_HALF * (2 + TILE_HEIGHT_WIDTH_RATIO);  // TODO simplify/combine equation w/above? make static . . .
//            screenY += ((i+k) * (TILE_HEIGHT_HALF/2));  // This would work if the array were staggered
            SDL_Rect rect = {screenX, screenY, TILE_WIDTH_HALF*2, TILE_HEIGHT_HALF*2};
            SDL_RenderCopy(&renderer.getRenderer(), map_[i][k], nullptr, &rect);
        }
    }
}
