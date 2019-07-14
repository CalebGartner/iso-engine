/*
 * Game Loop articles/resources/patterns/explanations/etc.
 * https://web.archive.org/web/20190506122532/http://gafferongames.com/post/fix_your_timestep/
 * https://dewitters.com/dewitters-gameloop/
 * https://web.archive.org/web/20141116170950/http://www.richardfine.co.uk/2012/10/unity3d-monobehaviour-lifecycle/
 */

#include "Game.h"


Game::Game(){
//    renderer_ = Renderer();
    running_ = false;
//    event_ = SDL_Event();
    level_ = std::make_unique<Level>(1);
}

bool Game::init() {
    // Each game engine component will have an init() function since SDL must startup first
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) return false;

    if (!Display::init()) return false;

    if (!renderer_.init()) return false;  // Renderer uses Display, init() after
    // TODO init other engine components as they are created

    if (!level_->init(renderer_)) return false;

    return true;
}

void Game::run() {
    running_ = true;  // TODO make SDL_bool ?

    // TODO put all the below into a struct? TickTimer? LoopTiming?
    Uint32 previous = SDL_GetTicks();  // milliseconds since SDL initialization
    Uint32 current = 0;
    Uint32 elapsed = 0;
    Uint32 lag = 0;
    Uint8 loops = 0;

    while (running_) {
        current = SDL_GetTicks();
        elapsed = current - previous;
        previous = current;
        lag += elapsed;

        processInput();

        loops = 0;
        while (lag >= Display::MS_PER_UPDATE && loops < MAX_FRAMESKIP) {  // fixed time-step
            update();  // option - Only update once/<RR times a second, but render @ RR
            lag -= Display::MS_PER_UPDATE;
            loops++;
        }

        // TODO make Game::render() function to access renderer_/level_/everything else
        renderer_.show();  // TODO pass lag to render - (lag / MSPerUpdate) - normalize the value between 0 and 1
    }

    shutdown();
}

void Game::update() {
    // TODO Deal w/event queue modified by proccessInput() and update/check buffers/spatial mapping(s)
}

void Game::processInput() {
    // TODO Use event queue - SDL has one by default
    while (SDL_PollEvent(&event_) != 0) {
        // User requests quit
        if (event_.type == SDL_QUIT) {
            running_ = false;
        }
    }
}

void Game::shutdown() {
    running_ = false;  // This is blocking . . . call from another thread? Is it caught from user input instead?
    renderer_.shutdown();
    SDL_Quit();
}

void Game::render() const {
    SDL_SetRenderDrawColor(&renderer_.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(&renderer_.getRenderer());

    // TODO render level tiles/player/other stuff
    renderer_.show();  // remove?

    SDL_RenderPresent(&renderer_.getRenderer());
}
