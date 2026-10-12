#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/WindowUtility.h"
#include "Gugu/Window/Color.h"

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

namespace EGameWindow
{
    enum Type
    {
        None,
        Sfml,
    };
}

struct EngineConfig
{
    // Application
    std::string applicationName;
    std::string applicationIcon;

    // Update
    int maxUpdateDeltaTimeMs;

    // Step
    bool useConstantStep;
    int constantStepTimeMs;

    // Resources
    bool useAssetsFullPaths;
    std::string pathAssets;
    std::string pathScreenshots;
    std::string defaultFont;
    std::string debugFont;
    bool defaultTextureSmooth;
    bool handleResourceDependencies;

    // Graphics
    EGameWindow::Type gameWindow;
    EWindowState windowState;
    int windowWidth;
    int windowHeight;
    bool maximizeWindow;
    bool enableVerticalSync;
    int framerateLimit;
    Color backgroundColor;

    // Audio
    std::string rootAudioMixerGroup;
    int maxSoundSourceCount;            // Total sources should not exceed 256.
    int maxMusicSourceCount;            // Total sources should not exceed 256.
    int audioListenerDistance;          // (Spatialization) Distance between the listener and the "game space".

    // Debug
    bool allowEngineLog;
    bool allowEngineDebugLog;
    bool allowConsole;
    bool showStats;
    bool showFPS;
    bool showImGui;


    EngineConfig()
    {
        applicationName = "Game Project";
        applicationIcon = "";

        maxUpdateDeltaTimeMs = 100;

        useConstantStep = true;
        constantStepTimeMs = 20;

        useAssetsFullPaths = false;
        pathAssets = "";
        pathScreenshots = "Screenshots";
        defaultFont = "";
        debugFont = "";
        defaultTextureSmooth = false;
        handleResourceDependencies = false;

        gameWindow = EGameWindow::Sfml;
        windowState = EWindowState::Windowed;
        windowWidth = 800;
        windowHeight = 600;
        maximizeWindow = false;
        enableVerticalSync = false;
        framerateLimit = 60;
        backgroundColor = Color(128, 128, 128, 255);

        rootAudioMixerGroup = "";
        maxSoundSourceCount = 240;   // Total tracks should not exceed 256
        maxMusicSourceCount = 16;    // Total tracks should not exceed 256
        audioListenerDistance = 800;

        allowEngineLog = true;
        allowEngineDebugLog = true;
        allowConsole = true;
        showStats = false;
        showFPS = false;
        showImGui = false;
    }
};

}   // namespace gugu
