#pragma once

#include <engine/Game.h>

// Adds the game's scenes and starts on the title; the scene manager drives them from there
class Game : public engine::Game
{
public:
    explicit Game(engine::GameConfig config);

private:
    void onInit() override;
};
