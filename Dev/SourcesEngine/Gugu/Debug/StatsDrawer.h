#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Sprite.hpp>   //TODO: use pointers + forward declaration
#include <SFML/Graphics/Text.hpp>   //TODO: use pointers + forward declaration
#include <SFML/Graphics/VertexArray.hpp>   //TODO: use pointers + forward declaration

#include <list>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineStats;
    struct FrameInfos;
    class Window;
}

namespace sf
{
    class RenderWindow;
    class Time;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class StatsDrawer
{
public:

    StatsDrawer();
    ~StatsDrawer();

    void DrawFPS(const sf::Time& loopTime, Window* window);
    void DrawStats(const FrameInfos& frameInfos, const sf::Time& renderTime, const sf::Time& loopTime, const EngineStats& engineStats, Window* window);

private:

    struct StatsSummary
    {
        float min = 99999999.f;
        float max = 0.f;
        float avg = 0.f;
    };

    void ComputeStatsSummary(const std::list<float>& statValues, StatsSummary& statsSummary);
    void DrawCurve(const std::list<float>& statValues, const StatsSummary& statsSummary, sf::VertexArray& curve, Vector2f position, sf::RenderWindow* renderWindow);
    void DrawHistogram(const std::list<float>& statValues, const StatsSummary& statsSummary, sf::VertexArray& curve, Vector2f position, sf::RenderWindow* renderWindow);
    
private:

    std::list<float> m_statFrameTimes;
    std::list<float> m_statDrawCalls;

    float m_curveHeight;

    sf::VertexArray m_borders;
    sf::VertexArray m_curveSteps;
    sf::VertexArray m_curveStepCount;
    sf::VertexArray m_curveUpdates;
    sf::VertexArray m_curveRenders;
    sf::VertexArray m_curveDrawCalls;

    sf::Text m_statTextStandaloneFPS;

    sf::Sprite m_statsBackground;
    sf::Text m_statTextFPS;
    sf::Text m_statTextDrawCalls;
    sf::Text m_statTextStepTime;
    sf::Text m_statTextUpdateTime;
    sf::Text m_statTextRenderTime;
    sf::Text m_statTextAnimations;
    sf::Text m_statTextParticleSystems;
    sf::Text m_statTextIsTracing;
    //sf::Text m_statTextIsInputAllowed;
};

}   // namespace gugu
