#include "PlayerState.h"

StillState PlayerState::Still = StillState();
MovingState PlayerState::Moving = MovingState();

void StillState::update(const Player &player) {

}

void MovingState::update(const Player &player) {
    int ticksSpentMoving = SDL_GetTicks() - start_;


}
