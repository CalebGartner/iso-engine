#ifndef ISO_ENGINE_GAME_H
#define ISO_ENGINE_GAME_H

#include "Renderer.h"

class Game {
public:
    Game();

    bool                        Init();
    void                        Run();
    void                        Update();
    void                        Shutdown();
    Renderer&                   GetRenderer();

    ~Game();

private:

    Renderer                    renderer;
    bool                        running = false;

};

inline Renderer& Game::GetRenderer() {
    return renderer;
}

//extern Game game;

#endif //ISO_ENGINE_GAME_H
