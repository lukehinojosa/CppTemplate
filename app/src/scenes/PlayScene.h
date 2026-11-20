#pragma once

#include <memory>
#include <vector>

#include <cyclone/cyclone.h>
#include <engine/FlyCamera.h>
#include <engine/Scene.h>

// A physics box, plus its pose before the last physics step so drawing can blend between steps
struct Box
{
    cyclone::RigidBody body;
    cyclone::BoxCollider collider;
    cyclone::Vector3 previousPosition;
    cyclone::Quaternion previousOrientation;
    Color color{};
};

// Drops physics boxes on a floor: where game code goes. Escape or P pauses.
class PlayScene : public engine::Scene
{
private:
    void onEnter() override;
    void onFixedUpdate(double dt) override;
    void onUpdate(double frameDt, double alpha) override;
    void onDraw3D() override;
    void onGui() override;

    void resetScene();
    void addBox(Vector3 position, Vector3 halfSize, Color color, Quaternion orientation = { 0.0f, 0.0f, 0.0f, 1.0f });
    void dropBox();

    // The world keeps pointers to the bodies and colliders, so each box lives at a fixed address
    cyclone::World mWorld;
    cyclone::PlaneCollider mGround;
    std::vector<std::unique_ptr<Box>> mBoxes;

    int mTowerHeight = 4;
    engine::FlyCamera mFlyCamera;
    float mAlpha = 0.0f;
};
