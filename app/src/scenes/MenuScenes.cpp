#include "MenuScenes.h"

#include <imgui.h>

#include <engine/Game.h>
#include <engine/SceneManager.h>

namespace
{
    const engine::SceneTransition kFade = engine::SceneTransition::fade(0.6f);

    // A small window in the middle of the screen, sized to its buttons
    bool beginCentredWindow(const char* name)
    {
        ImGui::SetNextWindowPos({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 40.0f }, ImGuiCond_Always, { 0.5f, 0.5f });
        return ImGui::Begin(name, nullptr,
                            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                                ImGuiWindowFlags_NoSavedSettings);
    }

    void drawCentredText(const char* text, int y, int size, Color color)
    {
        DrawText(text, (GetScreenWidth() - MeasureText(text, size)) / 2, y, size, color);
    }
}

void TitleScene::onUpdate(double, double)
{
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        getScenes().switchTo("play", mTowerHeight, kFade);
    }
}

void TitleScene::onDraw2D()
{
    drawCentredText(getConfig().title.c_str(), GetScreenHeight() / 2 - 90, 48, RAYWHITE);
    drawCentredText("Press Enter to play", GetScreenHeight() / 2 - 30, 20, GRAY);
}

void TitleScene::onGui()
{
    if (beginCentredWindow("Title"))
    {
        ImGui::SetNextItemWidth(160.0f);
        ImGui::SliderInt("##height", &mTowerHeight, 1, 10, "Tower of %d");
        if (ImGui::Button("Play", { 160.0f, 0.0f }))
        {
            getScenes().switchTo("play", mTowerHeight, kFade);
        }
#ifndef __EMSCRIPTEN__
        if (ImGui::Button("Quit", { 160.0f, 0.0f }))
        {
            requestQuit();
        }
#endif
    }
    ImGui::End();
}

void PauseScene::onUpdate(double, double)
{
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
    {
        getScenes().pop();
    }
}

void PauseScene::onDraw2D()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    drawCentredText("Paused", GetScreenHeight() / 2 - 90, 40, RAYWHITE);
}

void PauseScene::onGui()
{
    if (beginCentredWindow("Paused"))
    {
        if (ImGui::Button("Resume", { 160.0f, 0.0f }))
        {
            getScenes().pop();
        }
        if (ImGui::Button("Restart", { 160.0f, 0.0f }))
        {
            // The same value the play scene was started with, so it restarts as it began
            getScenes().switchTo("play", getScenes().find("play")->getParameter(), kFade);
        }
        if (ImGui::Button("Title", { 160.0f, 0.0f }))
        {
            getScenes().switchTo("title", kFade);
        }
#ifndef __EMSCRIPTEN__
        if (ImGui::Button("Quit", { 160.0f, 0.0f }))
        {
            requestQuit();
        }
#endif
    }
    ImGui::End();
}
