#pragma once

#include <engine/Game.h>

#include "Session.h"

// Adds the game's scenes and starts on the title; the scene manager drives them from there. Owns what the scenes
// share for the whole run, which outlasts any one scene.
class Game : public engine::Game
{
public:
    explicit Game(engine::GameConfig config);

private:
    void onInit() override;

    Session mSession;
};
