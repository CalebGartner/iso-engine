#include "Game.h"

//Game game = Game();
//Game* gameInstance = &game;

Game::Game()  {
    // TODO init other engine components as they are created
    renderer = Renderer();
}

bool Game::Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ERROR", SDL_GetError(), nullptr);
        return false;
    }

    if (!renderer.Init()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ERROR", SDL_GetError(), nullptr);
        return false;
    }

    return true;
}

void Game::Shutdown() {
    running = false;
    renderer.Shutdown();
    SDL_Quit();
}

void Game::Run() {
    running = true;
    while (running) {
        Update();  // TODO
    }
}

void Game::Update() {
    // renderer.Show() ?
}

Game::~Game() {
    Shutdown();
}
