#include <stdio.h>
#include "Game.h"

int main() {
    // TODO extern game as global pointer/ref - init game from . . . ? main?
    Game gameInstance;
    Game* game = &gameInstance;
    double previous = SDL_GetTicks();
    if (!game->init()) {
        game->shutdown();
        double current = SDL_GetTicks();
        double elapsed = current - previous;
        return 1;
    }

    game->run(); // main loop entry

    return 0;
}
