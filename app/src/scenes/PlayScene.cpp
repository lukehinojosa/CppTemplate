#include "PlayScene.h"

#include <imgui.h>
#include <raymath.h>
#include <rlgl.h>

#include <engine/Conversions.h>

using engine::toCyclone;
using engine::toRaylib;

void PlayScene::onEnter()
{
    // Entered from the title or a restart, so always a fresh scene; returning from pause resumes instead.
    // The title passes the tower's height in.
    mTowerHeight = getParameter(4);
    mSession.gamesStarted++;
    const float height = (float)mTowerHeight;
    getCamera().position = { 9.0f, 3.0f + height, 9.0f + height };
    getCamera().target = { 0.0f, height / 2.0f, 0.0f };
    resetScene();
}

void PlayScene::resetScene()
{
    // Clear the world's pointers before the boxes they point at are destroyed
    mWorld.bodies.clear();
    mWorld.colliders.clear();
    mWorld.clearCoherence();
    mBoxes.clear();
    mWorld.colliders.push_back(&mGround);

    for (int i = 0; i < mTowerHeight; i++)
    {
        addBox({ 0.0f, 0.5f + (float)i, 0.0f }, { 0.5f, 0.5f, 0.5f }, i % 2 == 0 ? SKYBLUE : DARKBLUE);
    }
    // Tilted and off-centre, so it lands on a corner and knocks the tower
    addBox({ 0.4f, (float)mTowerHeight + 5.0f, 0.3f }, { 0.6f, 0.3f, 0.4f }, ORANGE, QuaternionFromEuler(0.6f, 0.3f, 0.4f));
}

void PlayScene::addBox(Vector3 position, Vector3 halfSize, Color color, Quaternion orientation)
{
    auto pBox = std::make_unique<Box>();
    const float mass = 8.0f * halfSize.x * halfSize.y * halfSize.z;
    pBox->body.setMass(mass);
    cyclone::Matrix3 inertia;
    inertia.setBlockInertiaTensor(toCyclone(halfSize), mass);
    pBox->body.setInertiaTensor(inertia); // without one, contacts can't make it spin
    pBox->body.setPosition(toCyclone(position));
    pBox->body.setOrientation(toCyclone(orientation));
    pBox->collider.halfSize = toCyclone(halfSize);
    pBox->collider.pBody = &pBox->body;
    pBox->previousPosition = pBox->body.getPosition();
    pBox->previousOrientation = pBox->body.getOrientation();
    pBox->color = color;

    mWorld.bodies.push_back(&pBox->body);
    mWorld.colliders.push_back(&pBox->collider);
    mBoxes.push_back(std::move(pBox));
}

void PlayScene::dropBox()
{
    const Vector3 position = { (float)GetRandomValue(-20, 20) / 10.0f, 8.0f, (float)GetRandomValue(-20, 20) / 10.0f };
    const Quaternion tilt = QuaternionFromEuler((float)GetRandomValue(0, 314) / 100.0f, 0.0f, (float)GetRandomValue(0, 314) / 100.0f);
    addBox(position, { 0.5f, 0.5f, 0.5f }, ColorFromHSV((float)GetRandomValue(0, 360), 0.6f, 0.9f), tilt);
    mSession.boxesDropped++;
}

void PlayScene::onFixedUpdate(double dt)
{
    for (const auto& pBox : mBoxes)
    {
        pBox->previousPosition = pBox->body.getPosition();
        pBox->previousOrientation = pBox->body.getOrientation();
    }
    mWorld.runPhysics((cyclone::real)dt);
}

void PlayScene::onUpdate(double frameDt, double alpha)
{
    mAlpha = (float)alpha;
    mFlyCamera.update(getCamera(), (float)frameDt);

    if (IsKeyPressed(KEY_R))
    {
        resetScene();
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        dropBox();
    }
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
    {
        push("pause");
    }
}

void PlayScene::onDraw3D()
{
    DrawPlane({ 0.0f, 0.0f, 0.0f }, { 30.0f, 30.0f }, Color{ 60, 64, 72, 255 });
    DrawGrid(30, 1.0f);

    for (const auto& pBox : mBoxes)
    {
        // Physics runs at a fixed rate, so draw each box between its last two poses
        const Vector3 position = Vector3Lerp(toRaylib(pBox->previousPosition), toRaylib(pBox->body.getPosition()), mAlpha);
        const Quaternion orientation = QuaternionSlerp(toRaylib(pBox->previousOrientation), toRaylib(pBox->body.getOrientation()), mAlpha);
        const Matrix pose = MatrixMultiply(QuaternionToMatrix(orientation), MatrixTranslate(position.x, position.y, position.z));
        const Vector3 size = Vector3Scale(toRaylib(pBox->collider.halfSize), 2.0f);
        const Color color = pBox->body.isAwake() ? pBox->color : ColorBrightness(pBox->color, -0.4f);

        rlPushMatrix();
        rlMultMatrixf(MatrixToFloatV(pose).v);
        DrawCubeV(Vector3Zero(), size, color);
        DrawCubeWiresV(Vector3Zero(), size, BLACK);
        rlPopMatrix();
    }
}

void PlayScene::onGui()
{
    ImGui::Begin("Game");
    ImGui::Text("Boxes: %d (%u awake)", (int)mBoxes.size(), mWorld.getAwakeCount());
    ImGui::Text("Contacts: %u", mWorld.getContactCount());
    if (ImGui::Button("Drop box (Space)"))
    {
        dropBox();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset (R)"))
    {
        resetScene();
    }
    ImGui::TextDisabled("Right mouse + WASD to fly the camera, Escape or P to pause");
    ImGui::End();
}
