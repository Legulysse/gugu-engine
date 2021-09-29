#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
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
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class StatsDrawer
{
public:

    StatsDrawer();
    ~StatsDrawer();

    void DrawFPS(const DeltaTime& timeSinceLastFrame, Window* window);
    void DrawStats(const FrameInfos& frameInfos, const DeltaTime& frameTime, const DeltaTime& timeSinceLastFrame, const EngineStats& engineStats, Window* window);

private:

    struct StatsSummary
    {
        int min = 99999999;
        int max = 0;
        int last = -1;
    };

    void ComputeStatsSummary(const std::list<int>& statValues, StatsSummary& statsSummary);
    void DrawCurve(const std::list<int>& statValues, const StatsSummary& statsSummary, sf::VertexArray& curve, Vector2f position, sf::RenderWindow* renderWindow);
    void DrawHistogram(const std::list<int>& statValues, const StatsSummary& statsSummary, sf::VertexArray& curve, Vector2f position, sf::RenderWindow* renderWindow);
    
private:

    std::list<int> m_statFrameTimes;
    std::list<int> m_statDrawCalls;

    float m_curveHeight;

    sf::VertexArray m_borders;
    sf::VertexArray m_curveSteps;
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
    sf::Text m_statTextIsTracing;
};

}   // namespace gugu
