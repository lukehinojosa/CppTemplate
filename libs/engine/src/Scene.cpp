#include "engine/Scene.h"

#include "engine/Game.h"

namespace engine
{
    Camera3D& Scene::getCamera() const
    {
        return mpGame->getCamera();
    }

    InputSystem& Scene::getInput() const
    {
        return mpGame->getInput();
    }

    AudioManager& Scene::getAudio() const
    {
        return mpGame->getAudio();
    }

    core::CallbackManager& Scene::getCallbacks() const
    {
        return mpGame->getCallbacks();
    }

    const GameConfig& Scene::getConfig() const
    {
        return mpGame->getConfig();
    }

    double Scene::getTime() const
    {
        return mpGame->getTime();
    }

    double Scene::getFixedDt() const
    {
        return mpGame->getFixedDt();
    }

    void Scene::requestQuit() const
    {
        mpGame->requestQuit();
    }
}
