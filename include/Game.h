#ifndef ISO_ENGINE_GAME_H
#define ISO_ENGINE_GAME_H


#include "SDL.h"
//#include "Renderer.h"
//#include "Level.h"
#include "Player.h"

// TODO logging (via SDL_Log?) and debug preprocessing/asserts

static Uint8 MAX_FRAMESKIP = 5;

class Game {
public:
    inline static std::string GameConfig = "q*bert.toml";

    Game();
    virtual ~Game() { shutdown(); };

    bool init();
    void run();
    void update();
    void render() const;
    void shutdown();
    const Renderer &getRenderer();

private:
    Renderer renderer_{};
    bool running_;
    SDL_Event event_{};
    // Will be automatically deleted when switching levels via reset()
    // TODO Load level by menu selection after that's implemented
    std::unique_ptr<Level> level_;
    std::unique_ptr<Player> player_;

    void processInput();
};

inline const Renderer &Game::getRenderer() { return renderer_; }

#endif //ISO_ENGINE_GAME_H
