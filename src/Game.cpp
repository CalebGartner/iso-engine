#include "Game.h"

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
    running = true;  // TODO make SDL_bool ?
    while (running) {
        Update();  // TODO
//        renderer.Show();
    }
}

void Game::Update() {
    while(SDL_PollEvent( &e ) != 0) {
        // User requests quit
         if(e.type == SDL_QUIT) {
             running = false;
         }
    }
    renderer.Show();
}

Game::~Game() {
    Shutdown();
}
