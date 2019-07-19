#include <Player.h>
#include "PlayerState.h"

StillState PlayerState::Still = StillState();
MovingState PlayerState::Moving = MovingState();

void StillState::update(Player &player) {
    // do nothing - or switch animation sprite
}

void MovingState::update(Player &player) {
    if (timesMoved_ == 0) {
        numFrames_ = static_cast<int>((static_cast<double>(MovingState::MS_TO_MOVE) / 1000.0) * static_cast<double>(Display::REFRESH_RATE));
    }

    double lag_factor = 0.5;
    double t = 1.0 / numFrames_;
    if (timesMoved_ < (numFrames_ / 2)) {
        player.x_ += (t * player.dX_) * (1.0 + lag_factor);
        player.y_ += (t * player.dY_) * (1.0 + lag_factor);
    } else {
        player.x_ += (t * player.dX_) * (1.0 - lag_factor);
        player.y_ += (t * player.dY_) * (1.0 - lag_factor);
    }


    if (timesMoved_ <= numFrames_) {
        ++timesMoved_;
    } else {  // reset
        timesMoved_ = 0;
        player.x_ = std::round(player.x_);
        player.y_ = std::round(player.y_);
        if (std::abs(player.x_) < 0.5) {
            player.x_ = 0;
        }
        if (std::abs(player.y_) < 0.5) {
            player.y_ = 0;
        }
        player.dX_ = 0;
        player.dY_ = 0;
        Player::state_ = &PlayerState::Still;
        // Throw onto queue
        SDL_zero(tileEvent_);
        tileEvent_.type = ISO_TILE_TOUCHED;
        tileEvent_.user.data1 = &player.x_;
        tileEvent_.user.data2 = &player.y_;
        SDL_PushEvent(&tileEvent_);
    }
}
