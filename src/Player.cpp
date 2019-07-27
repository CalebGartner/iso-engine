#include "Player.h"
SDL_Rect Player::rect_ = SDL_Rect();
PlayerState *Player::state_ = &PlayerState::Still;

bool Player::init(const Renderer &renderer, const cpptoml::table &config) {
    // TODO check for out-of-bounds grid position
    auto start = *(config.get_qualified_array_of<int64_t>("player.start"));
    x_ = startX_ = start[0];
    y_ =  startY_ = start[1];
    rect_.x = getScreenX(x_);
    rect_.y = getScreenY(y_);

    rect_.w = rect_.h = static_cast<int>(std::min(TILE_WIDTH, TILE_HEIGHT) * PLAYER_TILE_RATIO);
    lives_ = *(config.get_qualified_as<int>("player.lives"));

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

void Player::processInput(const SDL_Event &event) {
    Player::state_ = &PlayerState::Moving;
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    dY_ = -1;
                    if ((y_ + dY_) % 2 == 0) {
                        dX_ = 1;
                    }
                    pivot_.x = x_ + 1;
                    pivot_.y = y_;
// TODO remember that y axis is flipped, should be 360/0 instead of 180 ?
                    rotationAngles_ = {100, 180};
                    break;
                case SDLK_DOWN:
                    dY_ = 1;
                    pivot_.x = x_;
                    if ((y_ + dY_) % 2 != 0) {
                        dX_ = -1;
                        pivot_.x = x_ + 1;
                    }
                    pivot_.y = y_ + 1;
                    rotationAngles_ = {180, 100};
                    break;
                case SDLK_LEFT:
                    dY_ = -1;
                    if ((y_ + dY_) % 2 != 0) {
                        dX_ = -1;
                    }
                    pivot_.x = x_ - 1;
                    pivot_.y = y_;
                    rotationAngles_ = {0, 80};
                    break;
                case SDLK_RIGHT:
                    dY_ = 1;
                    pivot_.x = x_ - 1;
                    if ((y_ + dY_) % 2 == 0) {
                        dX_ = 1;
                        pivot_.x = x_;
                    }
                    pivot_.y = y_ + 1;
                    rotationAngles_ = {80, 0};
                    break;
                default:
                    Player::state_ = &PlayerState::Still;
                    break;  // TODO play sound for invalid command
            }
}

void Player::render(const Renderer &renderer) const {
//    rect_.x = getScreenX();
//    rect_.y = getScreenY();
    SDL_RenderCopy(&renderer.getRenderer(), playerTexture_.get(), nullptr, &rect_);
//    SDL_RenderCopyEx(&renderer.getRenderer(), playerTexture_.get(), nullptr, &rect_, angle_, &pivot_, SDL_FLIP_NONE);
}

int Player::getScreenX(double x) const {
    int screenX = static_cast<int>(std::llrint(x - Level::gridOrigin_.x)) * (TILE_WIDTH);
    if (y_ % 2 == 0) {
        screenX -= (TILE_WIDTH / 2);  // * (y_%2)  ?
    }
    int pad = (TILE_WIDTH - rect_.w) / 2;
    screenX += pad;
    return screenX;

//    double screenX = (x_ - y_) * ((TILE_WIDTH/2));
//    screenX += (Renderer::WindowWidth / 2.0) - static_cast<int>((TILE_WIDTH/2) * PLAYER_TILE_RATIO);
//    return screenX;
}

int Player::getScreenY(double y) const {
    int vertical_correction = static_cast<int>(std::lrint(static_cast<float>(TILE_HEIGHT) * (1.0 - TILE_VERTICAL_STAGGER)));
    int screenY = static_cast<int>(std::llrint(y - Level::gridOrigin_.y)) * vertical_correction;
    screenY = screenY - static_cast<int>(std::lrint(static_cast<float>(TILE_HEIGHT) * (2 * TILE_VERTICAL_STAGGER)));
    int pad = (TILE_HEIGHT - rect_.h) / 2;
    screenY -= pad;
    return screenY;

    // *1.5 is necessary since each tile extends "downwards" - i.e., each one is a hexagonal tile, not a simple diamond
//    double screenY = (x_ + y_) * static_cast<int>((TILE_HEIGHT/2) * 1.5);
    // Offsetting the y values by this amount helps keep player sprite at the 'top' of the grid tile it's on
//    screenY -= static_cast<int>((TILE_HEIGHT/2) * (2.65));
//    return screenY;
}

void Player::update() {
    Player::state_->update(*this);
}

void Player::returnToStart() {
    x_ = startX_;
    y_ = startY_;
}
