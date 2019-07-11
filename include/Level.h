#ifndef ISO_ENGINE_LEVEL_H
#define ISO_ENGINE_LEVEL_H

#include <string>
#include "cpptoml.h"
#include "SDL.h"
// TODO double buffer the map?
class Level {
public:
    explicit Level(Uint32 levelID);

    static std::string gameConfig_;


private:
    Uint32 levelID_;
    Uint32 scorePerTile_ = 30;
    Uint32 bonus_ = 1000;
    Uint32 minScreenTileWidth_;
    Uint32 minScreenTileHeight_;

    std::string tileTouched_;
    std::string tileUntouched_;
};

#endif //ISO_ENGINE_LEVEL_H
