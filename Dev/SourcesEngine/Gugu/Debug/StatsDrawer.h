#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Sprite.hpp>   //TODO: use pointers + forward declaration
#include <SFML/Graphics/Text.hpp>   //TODO: use pointers + forward declaration

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
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class StatsDrawer
{
public:

    StatsDrawer();
    virtual ~StatsDrawer();

    void DrawStats(const FrameInfos& kFrameInfos, const DeltaTime& _kFrameTime, const DeltaTime& _kTimeSinceLastFrame, const EngineStats& engineStats, Window* window);
    void DrawFPS(const DeltaTime& _kTimeSinceLastFrame, Window* window);

protected:

    std::list<int> m_statFrameTimes;
    std::list<int> m_statDrawCalls;

    sf::Text m_statTextStandaloneFPS;

    sf::Sprite m_statsBackground;
    sf::Text m_statTextFameTime;
    sf::Text m_statTextFameTimeAverage;
    sf::Text m_statTextFameTimeMin;
    sf::Text m_statTextFameTimeMax;
    sf::Text m_statTextFPS;
    sf::Text m_statTextDrawCalls;
    sf::Text m_statTextStepTime;
    sf::Text m_statTextUpdateTime;
    sf::Text m_statTextRenderTime;
    sf::Text m_statTextIsTracing;
};

}   // namespace gugu
