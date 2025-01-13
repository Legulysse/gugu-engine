////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Debug/StatsDrawer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Renderer.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/System/String.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Debug/Trace.h"
#include "Gugu/Debug/EngineStats.h"

#include <SFML/Graphics/RenderWindow.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

StatsDrawer::StatsDrawer()
{
    m_curveHeight = 50.f;

    if (!GetResources()->GetDebugFont())
        return;

    // TODO: get the value from a constant, or add a resize mechanism when needed.
    int curvePoints = 150;
    m_statFrameTimes.resize(150);
    m_statDrawCalls.resize(150);

    sf::Font* font = GetResources()->GetDebugFont()->GetSFFont();
    int fontSize = 15;

    sf::Color colorDefault = sf::Color(255, 255, 255, 255);
    sf::Color colorCurveStepTimes = sf::Color(70, 200, 255, 255);
    sf::Color colorCurveUpdateTimes = sf::Color(255, 140, 50, 255);
    sf::Color colorCurveRenderTimes = sf::Color(50, 220, 50, 255);
    sf::Color colorCurveDrawCalls = sf::Color(255, 255, 70, 255);
    sf::Color colorCurveStepCount = sf::Color(255, 50, 255, 255);

    m_borders.setPrimitiveType(sf::PrimitiveType::Lines);

    m_curveSteps.setPrimitiveType(sf::PrimitiveType::Lines);
    m_curveSteps.resize(curvePoints * 2);
    for (size_t i = 0; i < m_curveSteps.getVertexCount(); ++i)
    {
        m_curveSteps[i].color = colorCurveStepTimes;
    }

    m_curveUpdates.setPrimitiveType(sf::PrimitiveType::Lines);
    m_curveUpdates.resize(curvePoints * 2);
    for (size_t i = 0; i < m_curveUpdates.getVertexCount(); ++i)
    {
        m_curveUpdates[i].color = colorCurveUpdateTimes;
    }

    m_curveRenders.setPrimitiveType(sf::PrimitiveType::Lines);
    m_curveRenders.resize(curvePoints * 2);
    for (size_t i = 0; i < m_curveRenders.getVertexCount(); ++i)
    {
        m_curveRenders[i].color = colorCurveRenderTimes;
    }

    m_curveDrawCalls.setPrimitiveType(sf::PrimitiveType::LineStrip);
    m_curveDrawCalls.resize(curvePoints);
    for (size_t i = 0; i < m_curveDrawCalls.getVertexCount(); ++i)
    {
        m_curveDrawCalls[i].color = colorCurveDrawCalls;
    }

    m_curveStepCount.setPrimitiveType(sf::PrimitiveType::LineStrip);
    m_curveStepCount.resize(curvePoints);
    for (size_t i = 0; i < m_curveStepCount.getVertexCount(); ++i)
    {
        m_curveStepCount[i].color = colorCurveStepCount;
    }

    m_statTextStandaloneFPS.setFillColor(colorDefault);
    m_statTextStandaloneFPS.setCharacterSize(20);
    m_statTextStandaloneFPS.setFont(*font);

    m_statTextFPS.setFillColor(colorDefault);
    m_statTextFPS.setCharacterSize(fontSize);
    m_statTextFPS.setFont(*font);

    m_statTextStepTime.setFillColor(colorCurveStepTimes);
    m_statTextStepTime.setCharacterSize(fontSize);
    m_statTextStepTime.setFont(*font);

    m_statTextUpdateTime.setFillColor(colorCurveUpdateTimes);
    m_statTextUpdateTime.setCharacterSize(fontSize);
    m_statTextUpdateTime.setFont(*font);

    m_statTextRenderTime.setFillColor(colorCurveRenderTimes);
    m_statTextRenderTime.setCharacterSize(fontSize);
    m_statTextRenderTime.setFont(*font);

    m_statTextDrawCalls.setFillColor(colorCurveDrawCalls);
    m_statTextDrawCalls.setCharacterSize(fontSize);
    m_statTextDrawCalls.setFont(*font);

    m_statTextAnimations.setFillColor(colorDefault);
    m_statTextAnimations.setCharacterSize(fontSize);
    m_statTextAnimations.setFont(*font);

    m_statTextParticleSystems.setFillColor(colorDefault);
    m_statTextParticleSystems.setCharacterSize(fontSize);
    m_statTextParticleSystems.setFont(*font);

    m_statTextIsTracing.setFillColor(colorDefault);
    m_statTextIsTracing.setCharacterSize(fontSize);
    m_statTextIsTracing.setString("(Tracing)");
    m_statTextIsTracing.setFont(*font);

    //m_statTextIsInputAllowed.setFillColor(colorDefault);
    //m_statTextIsInputAllowed.setCharacterSize(fontSize);
    //m_statTextIsInputAllowed.setString("Input Allowed");
    //m_statTextIsInputAllowed.setFont(*font);
}

StatsDrawer::~StatsDrawer()
{
}

void StatsDrawer::ComputeStatsSummary(const std::list<float>& statValues, StatsSummary& statsSummary)
{
    float avg = 0.f;
    int avgCount = 0;

    for (float value : statValues)
    {
        if (ApproxSuperiorOrEqualToZero(value, math::Epsilon6))
        {
            statsSummary.max = Max(statsSummary.max, value);
            statsSummary.min = Min(statsSummary.min, value);

            avg += value;
            ++avgCount;
        }
    }

    statsSummary.avg = avgCount > 0 ? avg / avgCount : 0.f;
}

void StatsDrawer::DrawCurve(const std::list<float>& statValues, const StatsSummary& statsSummary, sf::VertexArray& curve, Vector2f position, sf::RenderWindow* renderWindow)
{
    float curveHeight = m_curveHeight;
    float pointOffset = 0.5f;

    float curveTopValue = Max(2.f, statsSummary.max);
    float curvePointScaleY = curveHeight / curveTopValue;

    int index = 0;
    for (float value : statValues)
    {
        value = Max(0.f, value);
        curve[index].position = Vector2f(pointOffset + position.x + index * 2, position.y + curveHeight - value * curvePointScaleY);
        ++index;
    }

    renderWindow->draw(curve);
}

void StatsDrawer::DrawHistogram(const std::list<float>& statValues, const StatsSummary& statsSummary, sf::VertexArray& curve, Vector2f position, sf::RenderWindow* renderWindow)
{
    float curveHeight = m_curveHeight;
    float pointOffset = 0.5f;

    float curveTopValue = Max(2.f, statsSummary.max);
    float curvePointScaleY = curveHeight / curveTopValue;

    int index = 0;
    for (float value : statValues)
    {
        value = Max(0.f, value);
        curve[index * 2].position = Vector2f(pointOffset + position.x + index * 2, position.y + curveHeight);
        curve[index * 2 + 1].position = Vector2f(pointOffset + position.x + index * 2, position.y + curveHeight - value * curvePointScaleY);
        ++index;
    }

    renderWindow->draw(curve);
}

void StatsDrawer::DrawFPS(const sf::Time& loopTime, Window* window)
{
    if (!GetResources()->GetDebugFont())
        return;

    float loopTimeMs = static_cast<float>(static_cast<double>(loopTime.asMicroseconds()) / 1000.0);
    int fps = static_cast<int>(1000 / ((loopTimeMs > 0) ? loopTimeMs : 1));

    m_statTextStandaloneFPS.setPosition(2.f, 2.f);
    m_statTextStandaloneFPS.setString("fps: " + ToString(fps));
    window->GetSFRenderWindow()->draw(m_statTextStandaloneFPS);
}

void StatsDrawer::DrawStats(const FrameInfos& frameInfos, const sf::Time& renderTime, const sf::Time& loopTime, const EngineStats& engineStats, Window* window)
{
    if (!GetResources()->GetDebugFont())
        return;

    float loopTimeMs = static_cast<float>(static_cast<double>(loopTime.asMicroseconds()) / 1000.0);
    int fps = static_cast<int>(1000 / ((loopTimeMs > 0) ? loopTimeMs : 1));

    float renderTimeMs = static_cast<float>(static_cast<double>(renderTime.asMicroseconds()) / 1000.0);

    // TODO: move this somwhere else.
    m_statFrameTimes.push_front(renderTimeMs);
    if (m_statFrameTimes.size() > engineStats.maxStatCount)
    {
        m_statFrameTimes.pop_back();
    }

    // TODO: move this somwhere else.
    m_statDrawCalls.push_front(static_cast<float>(frameInfos.statDrawCalls));
    if (m_statDrawCalls.size() > engineStats.maxStatCount)
    {
        m_statDrawCalls.pop_back();
    }

    sf::RenderWindow* renderWindow = window->GetSFRenderWindow();

    StatsSummary statsSummarySteps;
    StatsSummary statsSummaryUpdates;
    StatsSummary statsSummaryRenders;
    StatsSummary statsSummaryDrawCalls;
    //StatsSummary statsSummaryStepCount;

    {
        GUGU_SCOPE_TRACE_MAIN("Compute Values");

        ComputeStatsSummary(engineStats.stepTimes, statsSummarySteps);
        ComputeStatsSummary(engineStats.updateTimes, statsSummaryUpdates);
        ComputeStatsSummary(engineStats.renderTimes, statsSummaryRenders);
        ComputeStatsSummary(m_statDrawCalls, statsSummaryDrawCalls);
        //ComputeStatsSummary(engineStats.stepCount, statsSummaryStepCount);
    }

    float textLineOffset = 22.f;
    float pointOffset = 0.5f;
    float curveHeight = m_curveHeight;
    float curveWidth = engineStats.maxStatCount * 2.f - 1;
    Vector2f positionCurves = Vector2f(5.f, 5.f);
    Vector2f positionTextLines = Vector2f(curveWidth + 12.f, 10.f);
    Vector2f backgroundSize = positionCurves + Vector2f(curveWidth + 220.f, 10.f + curveHeight + (curveHeight + 10.f) * 2);

    {
        GUGU_SCOPE_TRACE_MAIN("Curves");

        // Background
        Texture* textureBackground = GetResources()->GetCustomTexture("GuguConsoleBackground");
        m_statsBackground.setTexture(*textureBackground->GetSFTexture());
        m_statsBackground.setScale(backgroundSize.x / textureBackground->GetSize().x, backgroundSize.y / textureBackground->GetSize().y);
        renderWindow->draw(m_statsBackground);

        // Curves
        DrawHistogram(engineStats.stepTimes, statsSummarySteps, m_curveSteps, positionCurves, renderWindow);
        //DrawCurve(engineStats.stepCount, statsSummaryStepCount, m_curveStepCount, positionCurves, renderWindow);
        DrawHistogram(engineStats.updateTimes, statsSummaryUpdates, m_curveUpdates, positionCurves + Vector2f(0.f, curveHeight + 10.f), renderWindow);
        DrawHistogram(engineStats.renderTimes, statsSummaryRenders, m_curveRenders, positionCurves + Vector2f(0.f, (curveHeight + 10.f) * 2), renderWindow);
        DrawCurve(m_statDrawCalls, statsSummaryDrawCalls, m_curveDrawCalls, positionCurves + Vector2f(0, (curveHeight + 10.f) * 2), renderWindow);

        // Borders
        if (m_borders.getVertexCount() == 0)
        {
            sf::Color colorBorders = sf::Color(255, 255, 255, 255);
            Vector2f pointOffsetBorderLeft = Vector2f(-1.f, pointOffset + 1.f);
            Vector2f pointOffsetBorderRight = Vector2f(1.f + curveWidth, pointOffset + 1.f);

            m_borders.resize(6);
            m_borders[0] = sf::Vertex(positionCurves + Vector2f(0.f, curveHeight) + pointOffsetBorderLeft, colorBorders);
            m_borders[1] = sf::Vertex(positionCurves + Vector2f(0.f, curveHeight) + pointOffsetBorderRight, colorBorders);
            m_borders[2] = sf::Vertex(positionCurves + Vector2f(0.f, curveHeight + (curveHeight + 10.f)) + pointOffsetBorderLeft, colorBorders);
            m_borders[3] = sf::Vertex(positionCurves + Vector2f(0.f, curveHeight + (curveHeight + 10.f)) + pointOffsetBorderRight, colorBorders);
            m_borders[4] = sf::Vertex(positionCurves + Vector2f(0.f, curveHeight + (curveHeight + 10.f) * 2) + pointOffsetBorderLeft, colorBorders);
            m_borders[5] = sf::Vertex(positionCurves + Vector2f(0.f, curveHeight + (curveHeight + 10.f) * 2) + pointOffsetBorderRight, colorBorders);
        }

        renderWindow->draw(m_borders);
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Text Infos");

        int lineCount = 0;

        // Step Times
        m_statTextStepTime.setPosition(positionTextLines.x, positionTextLines.y + textLineOffset * lineCount);
        m_statTextStepTime.setString(StringFormat("step: {0} ms,  max: {1} ms", ToStringf(statsSummarySteps.avg, 2), ToStringf(statsSummarySteps.max, 2)));
        renderWindow->draw(m_statTextStepTime);
        ++lineCount;

        // Update Times
        m_statTextUpdateTime.setPosition(positionTextLines.x, positionTextLines.y + textLineOffset * lineCount);
        m_statTextUpdateTime.setString(StringFormat("update: {0} ms,  max: {1} ms", ToStringf(statsSummaryUpdates.avg, 2), ToStringf(statsSummaryUpdates.max, 2)));
        renderWindow->draw(m_statTextUpdateTime);
        ++lineCount;

        // Render Times
        m_statTextRenderTime.setPosition(positionTextLines.x, positionTextLines.y + textLineOffset * lineCount);
        m_statTextRenderTime.setString(StringFormat("render: {0} ms,  max: {1} ms", ToStringf(statsSummaryRenders.avg, 2), ToStringf(statsSummaryRenders.max, 2)));
        renderWindow->draw(m_statTextRenderTime);
        ++lineCount;

        // Draw Calls
        m_statTextDrawCalls.setPosition(positionTextLines.x, positionTextLines.y + textLineOffset * lineCount);
        m_statTextDrawCalls.setString(StringFormat("draw calls: {0}  tris: {1}", frameInfos.statDrawCalls, frameInfos.statTriangles));
        renderWindow->draw(m_statTextDrawCalls);
        ++lineCount;

        // Fps
        m_statTextFPS.setPosition(positionTextLines.x, positionTextLines.y + (textLineOffset) * lineCount);
        m_statTextFPS.setString(StringFormat("fps: {0}", fps));
        renderWindow->draw(m_statTextFPS);
        ++lineCount;

        // Animation Count
        m_statTextAnimations.setPosition(positionTextLines.x, positionTextLines.y + textLineOffset * lineCount);
        m_statTextAnimations.setString(StringFormat("animations: {0}", engineStats.animationCount));
        renderWindow->draw(m_statTextAnimations);
        ++lineCount;

        // Particle System Count
        m_statTextParticleSystems.setPosition(positionTextLines.x, positionTextLines.y + textLineOffset * lineCount);
        m_statTextParticleSystems.setString(StringFormat("particle systems: {0}", engineStats.particleSystemCount));
        renderWindow->draw(m_statTextParticleSystems);
        ++lineCount;

        //// IsInputAllowed
        //if (window->IsInputAllowed())
        //{
        //    m_statTextIsInputAllowed.setPosition(positionTextLines.x, positionTextLines.y + textLineOffset * lineCount);
        //    renderWindow->draw(m_statTextIsInputAllowed);
        //    ++lineCount;
        //}

        // Is Tracing
        if (engineStats.isTracing)
        {
            m_statTextIsTracing.setPosition(positionTextLines.x, positionTextLines.y + textLineOffset * lineCount);
            renderWindow->draw(m_statTextIsTracing);
            ++lineCount;
        }
    }
}

}   // namespace gugu
