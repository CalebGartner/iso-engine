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
    if (timesMoved_ == 0) {
        numFrames_ = static_cast<int>((static_cast<double>(MovingState::MS_TO_MOVE) / 1000.0) * static_cast<double>(Display::REFRESH_RATE));
    }

    double lag_factor = 0.3;
    int dX = (player.dX_ != 0) ? player.dX_ : (1 * player.dY_);
    int dY = (player.dY_ != 0) ? player.dY_ : (1 * player.dX_);
    if (player.dX_ == -1 || player.dY_ == -1) {
        dY *= -1;
        dX *= -1;
        lag_factor *= -1;
    }
    double t = 1.0 / numFrames_;
    if (timesMoved_ < (numFrames_ / 2)) {
        player.x_ += (t * (player.dX_ * (1.0 + lag_factor))) - (dX * t);
        player.y_ += (t * (player.dY_ * (1.0 + lag_factor))) - (dY * t);
    } else {
        player.x_ += (t * (player.dX_ * (1.0 - lag_factor))) + (dX * t);
        player.y_ += (t * (player.dY_ * (1.0 - lag_factor))) + (dY * t);
    }

    if (timesMoved_ <= numFrames_) {
        ++timesMoved_;
    } else {  // reset
        player.x_ = std::round(player.x_);
        player.y_ = std::round(player.y_);
        player.dX_ = 0;
        player.dY_ = 0;
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
    // AUDIO - fire event
//    deathEvent_.type = Audio::ISO_AUDIO_EVENT;
//    deathEvent_.user.code = 0;
//    deathEvent_.user.data1 = &audioFile_;
//    SDL_PushEvent(&deathEvent_);

    player.returnToStart();
    player.lives_ -= 1;  // TODO trigger event if it's 0?
    Player::state_ = &PlayerState::Still;
}
