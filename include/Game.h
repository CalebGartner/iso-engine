#ifndef ISO_ENGINE_GAME_H
#define ISO_ENGINE_GAME_H

#include "SDL_events.h"
#include "Renderer.h"
// TODO logging (via SDL_Log?) and debug preprocessing/asserts

static Uint8 MAX_FRAMESKIP = 5;

class Game {
public:
    // TODO init other engine components as they are created

    virtual ~Game() { shutdown(); };

    bool init();
    void run();
    void update();
    void shutdown();
    const Renderer &getRenderer();

private:
    Renderer renderer_;  // TODO make this a shared_ptr/ref to stack variable so everything is deleted properly?
    bool running_ = false;
    SDL_Event event_;  // replace when coding input handling . . .

    void processInput();
};

inline const Renderer &Game::getRenderer() { return renderer_; }

#endif //ISO_ENGINE_GAME_H
