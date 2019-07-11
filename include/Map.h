#ifndef ISO_ENGINE_MAP_H
#define ISO_ENGINE_MAP_H


// The 'Map' should load images to tiles and set the tile's position.
// Eventually load level from config file
// Should it also control the viewport? camera class?

class Map {
public:
    // TODO create Window class - handle resizing, fullscreen, etc. ?
    // TODO create Level class (each level has a 'Map' and associated config variables) - give Game a level_ variable?
    // TODO create 'Layer' class for levels?
//    friend class Renderer;

    virtual ~Map() { shutdown(); };

    bool init();
    void shutdown();

private:

};


#endif //ISO_ENGINE_MAP_H
