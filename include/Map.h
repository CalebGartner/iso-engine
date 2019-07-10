#include <utility>

#ifndef ISO_ENGINE_MAP_H
#define ISO_ENGINE_MAP_H

#include "Renderer.h"

// The 'Map' should load images to tiles and set the tile's position.
// Eventually load level from config file
// Should it also control the viewport? camera class?

class Map {
public:
    // TODO make 'friend' to Renderer?
    // TODO create Window class - handle resizing, fullscreen, etc.
    // TODO create Level class (each level has a 'Map' and associated config variables) - give Game a level_ variable?

    explicit Map(std::string game = "q*bert.toml")
    : gameFile_(std::move(game))
    {}

    virtual ~Map() { shutdown(); };

    bool init();
    void shutdown();

private:
    std::string gameFile_;

};


#endif //ISO_ENGINE_MAP_H
