#ifndef ISO_ENGINE_TILE_H
#define ISO_ENGINE_TILE_H


#include <SDL_render.h>

class Tile {
public:
    static int width;
    static int height;

    void                        Draw(SDL_Renderer* renderer);
    void                        setX(int x_position);
    void                        setY(int y_position);

private:
    // Coordinates and destination
    int x;
    int y;
    SDL_Rect*                   targetRect;
};


#endif //ISO_ENGINE_TILE_H
