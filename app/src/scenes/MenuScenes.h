#pragma once

#include <engine/Scene.h>

#include "../Session.h"

// The first screen: the game's name, what's happened so far this run, and a way in
class TitleScene : public engine::Scene
{
public:
    explicit TitleScene(const Session& session) : mSession(session) {}

private:
    void onUpdate(double frameDt, double alpha) override;
    void onDraw2D() override;
    void onGui() override;

    const Session& mSession;

    // Handed to the play scene when it starts
    int mTowerHeight = 4;
};

// Drawn over the paused game, which stays on screen behind it
class PauseScene : public engine::Scene
{
private:
    bool isOverlay() const override { return true; }
    void onUpdate(double frameDt, double alpha) override;
    void onDraw2D() override;
    void onGui() override;
};
