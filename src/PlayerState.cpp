#include <Player.h>
#include <Audio.h>
#include "PlayerState.h"

StillState PlayerState::Still = StillState();
MovingState PlayerState::Moving = MovingState();
DeadState PlayerState::Dead = DeadState();

void StillState::update(Player &player) {
    // do nothing - or switch animation sprite
}

void MovingState::update(Player &player) {
    // TODO just update the angle here? - angle depends on key pressed
    if (timesMoved_ == 0) {
        numFrames_ = static_cast<int>((static_cast<double>(MovingState::MS_TO_MOVE) / 1000.0) * static_cast<double>(Display::REFRESH_RATE));
        player.angleIncrement_ = (player.rotationAngles_.y - player.rotationAngles_.x) / numFrames_;
    }

    player.angle_ = player.rotationAngles_.x + (player.angleIncrement_ * timesMoved_);
    // TODO fix player.rect_ somehow . . .
    // + player.startX_ . . .
    Player::rect_.x = ((TILE_HEIGHT/2) * std::cos(player.angle_)) + player.getScreenX(static_cast<double>(player.pivot_.x));
    Player::rect_.y = ((TILE_HEIGHT/2) * std::sin(player.angle_)) + player.getScreenY(static_cast<double>(player.pivot_.y));

    if (timesMoved_ <= numFrames_) {
        ++timesMoved_;
    } else {  // reset
        player.x_ = Player::rect_.x = std::round(player.x_);
        player.y_ = Player::rect_.y = std::round(player.y_);
        player.dX_ = 0;
        player.dY_ = 0;
        player.angle_ = 0;
        player.angleIncrement_ = 0;
        SDL_zero(player.pivot_);
        timesMoved_ = 0;
        Player::state_ = &PlayerState::Still;
        // Push onto queue
        SDL_zero(tileEvent_);
        tileEvent_.type = ISO_TILE_TOUCHED;
        tileEvent_.user.data1 = &player;
        SDL_PushEvent(&tileEvent_);
    }
}

void DeadState::update(Player &player) {
    player.returnToStart();
    player.lives_ -= 1;

    SDL_zero(event_);
    event_.type = ISO_UI_EVENT;
    event_.user.code = 2;
    event_.user.data1 = &player.lives_;
    SDL_PushEvent(&event_);

    Player::state_ = &PlayerState::Still;
}
