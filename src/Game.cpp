/*
 * Game Loop articles/resources/patterns/explanations/etc.
 * https://web.archive.org/web/20190506122532/http://gafferongames.com/post/fix_your_timestep/
 * https://dewitters.com/dewitters-gameloop/
 * https://web.archive.org/web/20141116170950/http://www.richardfine.co.uk/2012/10/unity3d-monobehaviour-lifecycle/
 */

#include "Game.h"


Game::Game(){
    running_ = false;
    level_ = std::make_unique<Level>(1);
    player_ = std::make_unique<Player>();
}

bool Game::init() {
    // Each game engine component will have an init() function since SDL must startup first
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) return false;

    if (!Display::init()) return false;

    if (!Audio::init()) return false;

    // Open TOML file for parsing
    std::shared_ptr<cpptoml::table> config;
    try {
        config = cpptoml::parse_file(EngineUtils::getResourcePath(GameConfig));
    } catch (const cpptoml::parse_exception &e) {
        SDL_Log("PARSE ERROR: %s", e.what());
        return false;
    }

    std::string musicFile = *(config->get_as<std::string>("music"));
    musicLoop_.reset(Audio::loadMixMusic(musicFile));

    if (!renderer_.init(*config)) return false;  // Renderer uses Display, init() after

    if (!level_->init(renderer_, *config)) return false;

    if (!player_->init(renderer_, *config)) return false;

    return true;
}

void Game::run() {
    running_ = true;

    // TODO put all the below into a struct? TickTimer? LoopTiming?
    Uint32 previous = SDL_GetTicks();  // milliseconds since SDL initialization
    Uint32 current = 0;
    Uint32 elapsed = 0;
    Uint32 lag = 0;
    Uint8 loops = 0;
    Mix_PlayMusic(musicLoop_.get(), -1);

    while (running_) {
        current = SDL_GetTicks();
        elapsed = current - previous;
        previous = current;
        lag += elapsed;

        processInput();

        loops = 0;
        while (lag >= Display::MS_PER_UPDATE && loops < MAX_FRAMESKIP) {  // fixed time-step
            update();  // Only update once/<RR times a second, but render @ RR
            lag -= Display::MS_PER_UPDATE;
            loops++;
        }
        SDL_Delay(Display::MS_PER_UPDATE - lag);
        render();  // TODO pass lag to render - (lag / MSPerUpdate) - normalize the value between 0 and 1
    }
}

void Game::update() {
    level_->update();
    player_->update();
}

void Game::processInput() {
    while (SDL_PollEvent(&event_) != 0) {
        // User requests quit
        if (event_.type == SDL_QUIT) {
            running_ = false;
        } else if (event_.type == SDL_KEYDOWN) {
            player_->processInput(event_);
        } else if (event_.type == ISO_TILE_TOUCHED) {
            level_->processInput(event_);
        }
    }
}

void Game::shutdown() {
    running_ = false;  // This is blocking . . . call from another thread? Is it caught from user input instead?
    musicLoop_.reset();
    renderer_.shutdown();
    level_->shutdown();
    Audio::shutdown();
    SDL_Quit();
}

void Game::render() const {
    SDL_SetRenderDrawColor(&renderer_.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(&renderer_.getRenderer());

    renderer_.show();
    level_->render(renderer_);
    player_->render(renderer_);

    SDL_RenderPresent(&renderer_.getRenderer());
}
