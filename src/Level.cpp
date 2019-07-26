#include "Level.h"
#include <cmath>

int TILE_WIDTH_HALF = 0;
int TILE_HEIGHT_HALF = 0;
double TILE_HEIGHT_WIDTH_RATIO = 0;
Uint32 ISO_TILE_TOUCHED = 0;

bool Level::init(const Renderer &renderer, const cpptoml::table &config) {
    auto anchor = *(config.get_array_of<int64_t >("origin"));
    xOffset_ = anchor[0];
    yOffset_ = anchor[1];
    scorePerTile_ = *(config.get_as<Uint32>("scorepertile"));
    bonus_ = *(config.get_as<Uint32>("bonus"));

    // Get the corresponding level table from the TOML file+
    auto tarr = config.get_table_array("level");
    std::shared_ptr<cpptoml::table> level;
    for (const auto &table : *tarr) {
        auto id = table->get_as<Uint32>("id");
        if (*id == levelID_) {
            level = table;
            break;
        }
    }

    // Load the specified tile textures
    std::string touched = *(config.get_qualified_as<std::string>("tiles.touched"));
    tileTouched_.reset(loadTexture(renderer, touched));
    if (!tileTouched_) return false;

    std::string untouched = *(config.get_qualified_as<std::string>("tiles.untouched"));
    tileUntouched_.reset(loadTexture(renderer, untouched));
    if (!tileUntouched_) return false;

    // Event setup
    ISO_TILE_TOUCHED = SDL_RegisterEvents(1);
    if (ISO_TILE_TOUCHED == ((Uint32) - 1)) return false;

    event_.type = ISO_UI_EVENT;
    event_.user.code = 0;
    event_.user.data1 = &levelID_;
    SDL_PushEvent(&event_);

    // Set window icon
    std::string iconPath = *(config.get_as<std::string>("windowicon"));
    SDL_Surface *icon = IMG_Load(EngineUtils::getResourcePath(iconPath).c_str());
    SDL_SetWindowIcon(&renderer.getWindow(), icon);
    SDL_FreeSurface(icon);

    // Calculate tile dimensions
    minScreenTileHeight_ = *(level->get_as<Uint32>("minscreentileheight"));
    int windowTileHeight = Renderer::WindowHeight / minScreenTileHeight_;

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
            auto it = map_[i].begin();
            switch (tileType) {  // TODO change to hash map/pairs or something else
                case 1:
                    map_[i].insert(it, tileUntouched_.get());
                    break;
                case 2:
                    map_[i].insert(it, nullptr);
                    break;
                case 3:
                    map_[i].insert(it, tileTouched_.get());
                    break;
                default:
                    map_[i].insert(it, nullptr);
                    break;
            }
        }
        map_[i].shrink_to_fit();  // Currently the map doesn't change size during runtime . . .
        ++i;
    }

    std::string untouchedAudioFile = *(config.get_qualified_as<std::string>("tiles.audio.untouched"));
    std::string touchedAudioFile = *(config.get_qualified_as<std::string>("tiles.audio.touched"));
    std::string offMapAudioFile = *(config.get_qualified_as<std::string>("tiles.audio.offmap"));
    tileTouchedSound_.reset(Audio::loadMixChunk(touchedAudioFile));
    tileUntouchedSound_.reset(Audio::loadMixChunk(untouchedAudioFile));
    offMapSound_.reset(Audio::loadMixChunk(offMapAudioFile));

    return true;
}

// TODO add to ISOUtils namespace?
SDL_Texture *Level::loadTexture(const Renderer &renderer, const std::string &resource) {
    // TODO load from vector .svg sprite sheet instead?
    auto surface = IMG_Load(EngineUtils::getResourcePath(resource).c_str());
    if (surface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", resource.c_str(), IMG_GetError());
        return nullptr;
    }
    auto texture = SDL_CreateTextureFromSurface(&renderer.getRenderer(), surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Level::render(const Renderer &renderer) const {
    for(std::vector<std::vector<SDL_Texture*>>::size_type i = 0; i != map_.size(); i++) {
        for(std::vector<SDL_Texture*>::size_type k = 0; k != map_[i].size(); k++) {
            if (map_[i][k] == nullptr) continue;
            // TODO xOffset_, yOffset_ - change these when the player moves to keep the 'camera' centered on the PC
            // TODO create functions for mapToScreen and screenToMap
            int screenX = ((i - xOffset_) - (k - yOffset_)) * (TILE_WIDTH_HALF);
            // *1.5 is necessary since each tile extends "downwards" - i.e., each one is a hexagonal tile, not a simple diamond
            int screenY = ((i - xOffset_) + (k - yOffset_)) * static_cast<int>(TILE_HEIGHT_HALF*1.5);
            screenX += (Renderer::WindowWidth / 2) - TILE_WIDTH_HALF;  // TODO make static . . .
            // Offsetting the y values by this amount helps correct the map when the tile height and width differ
            screenY -= static_cast<int>(TILE_HEIGHT_HALF * (2 + TILE_HEIGHT_WIDTH_RATIO));  // TODO simplify/combine equation w/above? make static . . .
//            screenY += ((i+k) * (TILE_HEIGHT_HALF/2));  // This would work if the array were staggered
            SDL_Rect rect = {screenX, screenY, TILE_WIDTH_HALF*2, TILE_HEIGHT_HALF*2};
            SDL_RenderCopy(&renderer.getRenderer(), map_[i][k], nullptr, &rect);
        }
    }
}

void Level::update() {
    if (event_.user.code == 1) {
        auto *player = (Player*)(event_.user.data1);
        double xx = (player->x_);
        double yy = (player->y_);

        auto _x = std::to_string(xx);
        auto x = std::strtol(_x.substr(0, _x.find('.') + 2).c_str(), nullptr, 10);
        auto y = std::lrint(std::rint(yy));
        auto _y = std::lrint(yy);
        auto __y = std::rint(yy);

        if (map_[x][y] == nullptr) {
            // Player has fallen off the map - change their state
            Player::state_ = &PlayerState::Dead;
            Mix_PlayChannel(-1, offMapSound_.get(), 0);
        } else {
            if (map_[x][y]  == tileUntouched_.get()) {
                // Tile has not been touched
                Mix_PlayChannel(-1, tileUntouchedSound_.get(), 0);
                map_[x][y] = tileTouched_.get();

                SDL_zero(event_);
                event_.type = ISO_UI_EVENT;
                event_.user.code = 1;
                event_.user.data1 = &scorePerTile_;
                SDL_PushEvent(&event_);
            } else {
                // Tile has already been touched
                Mix_PlayChannel(-1, tileTouchedSound_.get(), 0);
            }
        }
    }
}

void Level::processInput(const SDL_Event &event) {
    // Player movement has been triggered, copy over the event data to handle on the next call to update()
    SDL_zero(event_);  // reset
    event_.type = event.user.type;
    event_.user.data1 = event.user.data1;
    event_.user.data2 = event.user.data2;
    event_.user.code = 1;
}

void Level::shutdown() {
    // This shouldn't be necessary, but what the heck
    tileTouchedSound_.reset();
    tileUntouchedSound_.reset();
    offMapSound_.reset();
}
