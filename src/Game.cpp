#include "Game.h"


bool Game::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ERROR", SDL_GetError(), nullptr);
        return false;
    }

    if (!renderer_.Init()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ERROR", SDL_GetError(), nullptr);
        return false;
    }

    return true;
}

void Game::run() {
    running_ = true;  // TODO make SDL_bool ?
    SDL_DisplayMode ddm;
    SDL_GetCurrentDisplayMode(0, &ddm);
    Uint32 interval = 1000 / ddm.refresh_rate;  // TODO move calculation to external static function in 'Display.h'
//    SDL_TimerID loopTimerID = SDL_AddTimer(interval, // TODO, nullptr); ?
    Uint32 previous = SDL_GetTicks();  // milliseconds since SDL initialization
    Uint32 lag = 0;
    while (running_) {
        Uint32 current = SDL_GetTicks();
        Uint32 elapsed = current - previous;
        previous = current;
        lag += elapsed;

//        processInput();

        while (lag >= interval)  // MS_PER_UPDATE ?
        {
            update();
            lag -= interval;
        }

//        render();  // TODO pass lag to render . . .
    }
//    while (running_) {
//        update();  // TODO
//        renderer_.Show();
//    }
}

void Game::update() {
    while(SDL_PollEvent( &event_ ) != 0) {
        // User requests quit
         if(event_.type == SDL_QUIT) {
             running_ = false;
         }
    }
    renderer_.Show();
}

void Game::shutdown() {
    running_ = false;
    renderer_.Shutdown();
    SDL_Quit();
}
