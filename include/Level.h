#ifndef ISO_ENGINE_LEVEL_H
#define ISO_ENGINE_LEVEL_H

#include "Map.h"


// TODO double buffer the map?
class Level {
public:
    explicit Level(Uint32 levelID)
    : levelID_(levelID)
    {};

    static std::string gameConfig_;


private:
    Uint32 scorePerTile_ = 30;
    Uint32 bonus_ = 1000;
    Uint32 levelID_;
};

#endif //ISO_ENGINE_LEVEL_H
