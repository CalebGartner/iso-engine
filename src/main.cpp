#include <stdio.h>
#include "Game.h"

int main() {
    // TODO extern game as global pointer/ref - init game from . . . ? main?
    Game gameInstance;
    Game* game = &gameInstance;
    if (!game->Init()) {
        game->Shutdown();
        return 1;
    }

    game->Run(); // main loop entry

    return 0;
}
