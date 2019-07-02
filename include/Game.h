#ifndef ISO_ENGINE_GAME_H
#define ISO_ENGINE_GAME_H

#include "Renderer.h"

static Uint16 MS_PER_UPDATE = 16;  // TODO move to Display class

class Game {
public:
    Game()
    : renderer_(Renderer()), running_(false), event_(SDL_Event())
    {}
    // TODO init other engine components as they are created

    virtual ~Game() { shutdown(); };

    bool init();
    void run();
    void update();
    void shutdown();
    Renderer &getRenderer();


private:

    Renderer renderer_;
    bool running_;
    SDL_Event event_;

};

inline Renderer &Game::getRenderer() {
    return renderer_;
}

#endif //ISO_ENGINE_GAME_H
