#include "Game.h"

int main() {
    // TODO extern game as global pointer/ref - init game from . . . ? main?
    Game gameInstance;
    Game* game = &gameInstance;

    if (!game->init()) {
        game->shutdown();
        return 1;
    }

    game->run(); // main loop entry

    return 0;
}
