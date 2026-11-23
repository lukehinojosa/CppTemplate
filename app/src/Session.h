#pragma once

// What the game remembers for as long as it runs, across every visit to every scene. Game owns it and hands a
// reference to each scene that needs it when adding the scene.
struct Session
{
    int gamesStarted = 0;
    int boxesDropped = 0;
};
