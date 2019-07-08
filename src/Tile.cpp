#include "Tile.h"


// TODO: create config file with pyramid base#/array of tile layout, enemies, power-ups, png for untouched/touched tiles, etc.

void Tile::Draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderDrawRect(renderer, targetRect);
}

void Tile::setX(int x_position) {
    x = x_position;
}

void Tile::setY(int y_position) {
    y = y_position;
}
