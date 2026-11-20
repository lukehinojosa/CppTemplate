#include "Game.h"

#include <utility>

#include "scenes/MenuScenes.h"
#include "scenes/PlayScene.h"

Game::Game(engine::GameConfig config) : engine::Game(std::move(config))
{
}

void Game::onInit()
{
    getScenes().add<TitleScene>("title");
    getScenes().add<PlayScene>("play");
    getScenes().add<PauseScene>("pause");
    getScenes().switchTo("title", engine::SceneTransition{ 0.0f, 0.5f });
}
