#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/Graphics/Color.hpp>

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
    int windowWidth;
    int windowHeight;
    bool fullscreen;
    bool maximizeWindow;
    bool enableVerticalSync;
    int framerateLimit;
    sf::Color backgroundColor;

    // Audio
    std::string rootAudioMixerGroup;
    int maxSoundSourceCount;     // Total sources should not exceed 256.
    int maxMusicSourceCount;     // Total sources should not exceed 256.

    // Debug
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
        windowWidth = 800;
        windowHeight = 600;
        fullscreen = false;
        maximizeWindow = false;
        enableVerticalSync = false;
        framerateLimit = 60;
        backgroundColor = sf::Color(128, 128, 128, 255);

        rootAudioMixerGroup = "";
        maxSoundSourceCount = 240;   // Total tracks should not exceed 256
        maxMusicSourceCount = 16;    // Total tracks should not exceed 256

        allowConsole = true;
        showStats = false;
        showFPS = false;
        showImGui = false;
    }
};

}   // namespace gugu
