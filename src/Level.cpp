#include "Level.h"

SDL_Point Level::gridOrigin_{0,0};


bool Level::init(const Renderer &renderer, const cpptoml::table &config) {
    auto anchor = *(config.get_array_of<int64_t>("origin"));
    gridOrigin_.x = anchor[0];
    gridOrigin_.y = anchor[1];

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

    minScreenTileHeight_ = *(config.get_as<int>("minscreentileheight"));

    event_.type = ISO_UI_EVENT;
    event_.user.code = 0;
    event_.user.data1 = &levelID_;
    SDL_PushEvent(&event_);

    // Set window icon
    std::string iconPath = *(config.get_as<std::string>("windowicon"));
    SDL_Surface *icon = IMG_Load(EngineUtils::getResourcePath(iconPath).c_str());
    SDL_SetWindowIcon(&renderer.getWindow(), icon);
    SDL_FreeSurface(icon);

    // Load the map
    auto levelMap = level->get_array_of<cpptoml::array>("map");
    map_ = std::vector<std::vector<std::forward_list<Tile>>>(levelMap->size());

    int y = 0;
    for (const auto &arr: *levelMap) {
        const auto line = arr->get_array_of<int64_t>();
        map_[y] = std::vector<std::forward_list<Tile>>(line->size());
        int x = 0;
        for (const auto &tileType: *line) {
            map_[y].emplace_back(std::forward_list<Tile>());
            SDL_Point tile_position{x, y};
            switch (tileType) {
                case 1:
                    map_[y][x].emplace_front(Tile(tile_position, Tile::Untouched.get()));
                    break;
                case 2:
                    map_[y][x].emplace_front(Tile(tile_position, nullptr));
                    break;
                case 3:
                    map_[y][x].emplace_front(Tile(tile_position, Tile::Touched.get()));
                    break;
                default:
                    map_[y][x].emplace_front(Tile(tile_position, nullptr));
                    break;
            }
            x++;
        }
        map_[y].shrink_to_fit();  // Currently the map doesn't change size during runtime . . .
        y++;
    }

    std::string offMapAudioFile = *(config.get_qualified_as<std::string>("tiles.audio.offmap"));
    DeathSound.reset(Audio::loadMixChunk(offMapAudioFile));

    return true;
}

// TODO limit rendering to screen/viewport limits
void Level::render(const Renderer &renderer) const {
    for(map_size_type y = 0; y != map_.size(); y++) {
        for(map_size_type x = 0; x != map_[y].size(); x++) {
            for (const Tile &tile : map_[y][x]) {  // Each grid coordinate represents a stack of tiles
                // TODO if x/y is > minscreentileheight/width, continue
                if (tile.texture_ != nullptr) {
                    tile.render(renderer);
                }
            }
        }
    }
}

void Level::update() {
    for(map_size_type y = 0; y != map_.size(); y++) {
        for(map_size_type x = 0; x != map_[y].size(); x++) {
            for (Tile &tile : map_[y][x]) {  // Each grid coordinate represents a 'stack'(std::forward_list) of tiles
                // TODO make utility functions getScreenX/Y() - add to Engineutils/Tile? take in SDL_Point gridOrigin, int gridX, int GridY param? param?
                int screenX = (tile.gridPosition_.x - gridOrigin_.x) * TILE_WIDTH;
                if (y % 2 == 0) {
                    screenX -= (TILE_WIDTH / 2);
                }
                tile.tileRect_.x = screenX;

                int vertical_correction = static_cast<int>(std::lrint(static_cast<float>(TILE_HEIGHT) * (1.0 - TILE_VERTICAL_STAGGER)));
                int screenY = (tile.gridPosition_.y - gridOrigin_.y) * vertical_correction;
                tile.tileRect_.y = screenY - static_cast<int>(std::lrint(static_cast<float>(TILE_HEIGHT) * (2 * TILE_VERTICAL_STAGGER)));
            }
        }
    }

//    if (event_.user.code == 1) {
//        auto *player = (Player*)(event_.user.data1);
//        double xx = (player->x_);
//        double yy = (player->y_);
//
//        auto _x = std::to_string(xx);
//        auto x = std::strtol(_x.substr(0, _x.find('.') + 2).c_str(), nullptr, 10);
//        auto y = std::lrint(std::rint(yy));
//        auto _y = std::lrint(yy);
//        auto __y = std::rint(yy);
//
//        if (map_[x][y] == nullptr) {
//            // Player has fallen off the map - change their state
//            Player::state_ = &PlayerState::Dead;
//            Mix_PlayChannel(-1, DeathSound.get(), 0);
//        } else {
//            if (map_[x][y]  == tileUntouched_.get()) {
//                // Tile has not been touched
//                Mix_PlayChannel(-1, tileUntouchedSound_.get(), 0);
//                map_[x][y] = tileTouched_.get();
//
//                SDL_zero(event_);
//                event_.type = ISO_UI_EVENT;
//                event_.user.code = 1;
//                event_.user.data1 = &scorePerTile_;
//                SDL_PushEvent(&event_);
//            } else {
//                // Tile has already been touched
//                Mix_PlayChannel(-1, tileTouchedSound_.get(), 0);
//            }
//        }
//    }
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
    DeathSound.reset();
}
