#ifndef ISO_ENGINE_PLAYERSTATE_H
#define ISO_ENGINE_PLAYERSTATE_H

#include "SDL_timer.h"

class Player;
class MovingState;
class StillState;


class PlayerState {
public:
    // These can be static since there's only one player
    static MovingState Moving;
    static StillState Still;

    virtual ~PlayerState() = default;
    virtual void update(const Player &player) = 0;
};

class StillState: public PlayerState {
public:
    void update(const Player &player) override;
};

class MovingState: public PlayerState {
public:
    void update(const Player &player) override;
private:
    inline static int MS_TO_MOVE = 1000;  // how long the player's movement from tile to tile takes
    inline static int timesMoved_ = 0;
    inline static int start_ = 0;
};

#endif //ISO_ENGINE_PLAYERSTATE_H
