#include "PathUtils.h"
#include "Level.h"

Level::Level(Uint32 levelID) {
    levelID_ = levelID;
    // Can't touch SDL here since it's initialized in Game::init()
    std::shared_ptr<cpptoml::table> config;
    try {
        config = cpptoml::parse_file(PathUtils::getResourcePath() + "q*bert.toml");  // make q*bert.toml a var somewhere);
    } catch (const cpptoml::parse_exception &e) {
        SDL_Log("PARSE ERROR: %s", e.what());
        return;
    }

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
    tileTouched_ = *(inner->get_as<std::string>("touched"));
    tileUntouched_ = *(inner->get_as<std::string>("untouched"));

    minScreenTileWidth_ = *(level->get_as<Uint32>("minscreentilewidth"));
    minScreenTileHeight_ = *(level->get_as<Uint32>("minscreentileheight"));

    auto map = level->get_array_of<cpptoml::array>("map");  // TODO make member variable?
//    auto arr = (*map)[0]->get_array_of<uint32_t>();
    for (const auto &line: *map) {
        auto hmm = line->as_array();
    }

}
