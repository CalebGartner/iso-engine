#include "Tile.h"


int TILE_WIDTH = 0;
int TILE_HEIGHT = 0;
float TILE_VERTICAL_STAGGER = 0.25;
Uint32 ISO_TILE_TOUCHED = 0;
static int SCORE_PER_TILE = 0;


bool Tile::init(const Renderer &renderer, const cpptoml::table &config) {
    // Event setup
    ISO_TILE_TOUCHED = SDL_RegisterEvents(1);
    if (ISO_TILE_TOUCHED == ((Uint32) - 1)) return false;

    SCORE_PER_TILE = *(config.get_as<int>("scorepertile"));

    // Load tile textures
    std::string touched = *(config.get_qualified_as<std::string>("tiles.touched"));
    Touched.reset(EngineUtils::loadTexture(&renderer.getRenderer(), touched));
    if (!Touched) return false;
    std::string untouched = *(config.get_qualified_as<std::string>("tiles.untouched"));
    Untouched.reset(EngineUtils::loadTexture(&renderer.getRenderer(), untouched));
    if (!Untouched) return false;

    // Calculate tile dimensions
    int minScreenTileHeight_ = *(config.get_as<int>("minscreentileheight"));
//    minScreenTileHeight_ = static_cast<int>(std::rint(static_cast<float>(minScreenTileHeight_) / 2.0));
    int windowTileHeight = Renderer::WindowHeight / minScreenTileHeight_;
    int w, h;
    SDL_QueryTexture(Touched.get(), nullptr, nullptr, &w, &h);
    TILE_HEIGHT = windowTileHeight;
    TILE_WIDTH = (w * TILE_HEIGHT) / h;  // width proportional to height of texture

    // Load audio
    std::string touchedAudioFile = *(config.get_qualified_as<std::string>("tiles.audio.touched"));
    TouchedSound.reset(Audio::loadMixChunk(touchedAudioFile));
    std::string untouchedAudioFile = *(config.get_qualified_as<std::string>("tiles.audio.untouched"));
    UntouchedSound.reset(Audio::loadMixChunk(untouchedAudioFile));

    return true;
}

void Tile::render(const Renderer &renderer) const {
    SDL_RenderCopy(&renderer.getRenderer(), texture_, nullptr, &tileRect_);
}

void Tile::update() {
    // TODO add tile 'state' ?
}

void Tile::shutdown() {
    Touched.reset();
    Untouched.reset();
    TouchedSound.reset();
    UntouchedSound.reset();
}

Tile::~Tile() {
    texture_ = nullptr;
}
