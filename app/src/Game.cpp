#include "Game.h"

#include <utility>

#include "scenes/MenuScenes.h"
#include "scenes/PlayScene.h"

Game::Game(engine::GameConfig config) : engine::Game(std::move(config))
{
}

void Game::onInit()
{
    // Data for the whole run goes in by reference here; data for one visit goes with switchTo(), like the tower height
    getScenes().add<TitleScene>("title", mSession);
    getScenes().add<PlayScene>("play", mSession);
    getScenes().add<PauseScene>("pause");
    getScenes().switchTo("title", engine::SceneTransition{ 0.0f, 0.5f });
}
