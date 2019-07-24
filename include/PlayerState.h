#ifndef ISO_ENGINE_PLAYERSTATE_H
#define ISO_ENGINE_PLAYERSTATE_H

class Player;
class MovingState;
class StillState;
class DeadState;

#include "Display.h"


class PlayerState {
public:
    // These can be static since there's only one player
    static MovingState Moving;
    static StillState Still;
    static DeadState Dead;

    virtual ~PlayerState() = default;
    virtual void update(Player &player) = 0;
};

class StillState: public PlayerState {
public:
    void update(Player &player) override;
};

class DeadState: public PlayerState {
public:
    void update(Player &player) override;
    SDL_Event deathEvent_;
};

class MovingState: public PlayerState {
public:
    void update(Player &player) override;
private:
    // how long the player's movement from tile to tile takes
    inline static int MS_TO_MOVE = 400;
    inline static int timesMoved_ = 0;
    // number of frames it takes to complete the movement based on the display refresh rate and MS_TO_MOVE
    inline static int numFrames_ = 0;
    inline static SDL_Event tileEvent_ = SDL_Event();
};

#endif //ISO_ENGINE_PLAYERSTATE_H
