#include "Player.h"

bool Player::init(const Renderer &renderer, const cpptoml::table &config) {
    // TODO check for out-of-bounds grid position
    auto start = *(config.get_qualified_array_of<int64_t>("player.start"));
    x_ = start[0];
    y_ = start[1];

    width_ = height_ = static_cast<int>(std::min(TILE_WIDTH_HALF, TILE_HEIGHT_HALF) * PLAYER_TILE_RATIO) * 2;

    std::string player_sprite = *(config.get_qualified_as<std::string>("player.sprite"));
    auto surface = IMG_Load(EngineUtils::getResourcePath(player_sprite).c_str());
    if (surface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", player_sprite.c_str(), IMG_GetError());
        return false;
    }
    // TODO change color key or set multiple color keys or set color key range?
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
    auto texture = SDL_CreateTextureFromSurface(&renderer.getRenderer(), surface);
    SDL_FreeSurface(surface);

    playerTexture_.reset(texture);
    if (!playerTexture_) return false;

    return true;
}

// TODO change to processInput? call from Game::processInput?
// TODO change state here
void Player::processInput() {
    // TODO check bounds
    // TODO change to slower movement of sprite to new coordinates
    SDL_Event event;
    if (SDL_PollEvent(&event) != 0) {
        // User requests quit
        if (event.type == SDL_KEYDOWN) {

            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    y_ -= 1; break;
                case SDLK_DOWN:
                    y_ += 1; break;
                case SDLK_LEFT:
                    x_ -= 1; break;
                case SDLK_RIGHT:
                    x_ += 1; break;
                default:
                    break;  // TODO play sound for invalid command
            }
        }
        // TODO flush/ignore event if still moving?
    }
}

void Player::render(const Renderer &renderer) const {
    int screenX = static_cast<int>(getScreenX());
    int screenY = static_cast<int>(getScreenY());
    SDL_Rect rect = {screenX, screenY, width_, height_};
    SDL_RenderCopy(&renderer.getRenderer(), playerTexture_.get(), nullptr, &rect);
}

double Player::getScreenX() const {
    double screenX = (x_ - y_) * (TILE_WIDTH_HALF);
    screenX += (Renderer::WindowWidth / 2.0) - static_cast<int>(TILE_WIDTH_HALF * PLAYER_TILE_RATIO);
    return screenX;
}

double Player::getScreenY() const {
    // *1.5 is necessary since each tile extends "downwards" - i.e., each one is a hexagonal tile, not a simple diamond
    double screenY = (x_ + y_) * static_cast<int>(TILE_HEIGHT_HALF * 1.5);
    // Offsetting the y values by this amount helps keep player sprite at the 'top' of the grid tile it's on
    screenY -= static_cast<int>(TILE_HEIGHT_HALF * (2.5 + TILE_HEIGHT_WIDTH_RATIO));
    return screenY;
}
