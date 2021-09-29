////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Debug/StatsDrawer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Debug/Trace.h"

#include <SFML/Graphics/RenderWindow.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

StatsDrawer::StatsDrawer()
{
    if (!GetResources()->GetDebugFont())
        return;

    sf::Font* pFont = GetResources()->GetDebugFont()->GetSFFont();
    int iFontSize = 15;

    m_statTextStandaloneFPS.setCharacterSize(20);
    m_statTextStandaloneFPS.setFont(*pFont);

    m_statTextFameTime.setCharacterSize(iFontSize);
    m_statTextFameTime.setFont(*pFont);

    m_statTextFameTimeAverage.setCharacterSize(iFontSize);
    m_statTextFameTimeAverage.setFont(*pFont);

    m_statTextFameTimeMin.setCharacterSize(iFontSize);
    m_statTextFameTimeMin.setFont(*pFont);

    m_statTextFameTimeMax.setCharacterSize(iFontSize);
    m_statTextFameTimeMax.setFont(*pFont);

    m_statTextFPS.setCharacterSize(iFontSize);
    m_statTextFPS.setFont(*pFont);

    m_statTextStepTime.setCharacterSize(iFontSize);
    m_statTextStepTime.setFont(*pFont);

    m_statTextUpdateTime.setCharacterSize(iFontSize);
    m_statTextUpdateTime.setFont(*pFont);

    m_statTextRenderTime.setCharacterSize(iFontSize);
    m_statTextRenderTime.setFont(*pFont);

    m_statTextDrawCalls.setCharacterSize(iFontSize);
    m_statTextDrawCalls.setFont(*pFont);

    m_statTextIsTracing.setCharacterSize(iFontSize);
    m_statTextIsTracing.setString("(Tracing)");
    m_statTextIsTracing.setFont(*pFont);
}

StatsDrawer::~StatsDrawer()
{
}

void StatsDrawer::DrawFPS(const DeltaTime& _kTimeSinceLastFrame, Window* window)
{
    if (!GetResources()->GetDebugFont())
        return;

    int iFPS = 1000 / ((_kTimeSinceLastFrame.ms() > 0) ? _kTimeSinceLastFrame.ms() : 1);

    sf::Color oColorText = sf::Color(255, 255, 255);

    m_statTextStandaloneFPS.setPosition(2.f, 2.f);
    m_statTextStandaloneFPS.setFillColor(oColorText);
    m_statTextStandaloneFPS.setString("FPS: " + ToString(iFPS));
    window->GetSFRenderWindow()->draw(m_statTextStandaloneFPS);
}

void StatsDrawer::DrawStats(const FrameInfos& kFrameInfos, const DeltaTime& _kFrameTime, const DeltaTime& _kTimeSinceLastFrame, const EngineStats& engineStats, Window* window)
{
    if (!GetResources()->GetDebugFont())
        return;

    sf::RenderWindow* renderWindow = window->GetSFRenderWindow();

    // Compute Values
    uint32 iNbCurvePointsMax = engineStats.maxStatCount;

    // Frame Times
    int iFrameTime = _kFrameTime.ms();
    int iMinFrameTime = 9999999;
    int iMaxFrameTime = 0;
    float fAvgFrameTime = 0.f;
    int iFPS = 1000 / ((_kTimeSinceLastFrame.ms() > 0) ? _kTimeSinceLastFrame.ms() : 1);

    // Draw Calls
    int iMaxDrawCalls = 1;

    // Step Times
    int minStepTime = 9999999;
    int maxStepTime = 0;
    int lastStepTime = -1;

    // Update Times
    int minUpdateTime = 9999999;
    int maxUpdateTime = 0;
    int lastUpdateTime = -1;

    // Render Times
    int minRenderTime = 9999999;
    int maxRenderTime = 0;
    int lastRenderTime = -1;

    {
        GUGU_SCOPE_TRACE_MAIN("Compute Values");

        // Frame Times
        m_statFrameTimes.push_front(iFrameTime);
        if (m_statFrameTimes.size() > iNbCurvePointsMax)
        {
            m_statFrameTimes.pop_back();
        }

        int iTotalFrameTime = 0;
        for (int iValue : m_statFrameTimes)
        {
            iMaxFrameTime = Max(iMaxFrameTime, iValue);
            iMinFrameTime = Min(iMinFrameTime, iValue);
            iTotalFrameTime += iValue;
        }

        fAvgFrameTime = (float)iTotalFrameTime / m_statFrameTimes.size();

        // Draw Calls
        m_statDrawCalls.push_front(kFrameInfos.statDrawCalls);
        if (m_statDrawCalls.size() > iNbCurvePointsMax)
        {
            m_statDrawCalls.pop_back();
        }

        for (int iValue : m_statDrawCalls)
        {
            iMaxDrawCalls = Max(iMaxDrawCalls, iValue);
        }

        // Step Times
        for (int value : engineStats.stepTimes)
        {
            maxStepTime = value >= 0 ? Max(maxStepTime, value) : maxStepTime;
            minStepTime = value >= 0 ? Min(minStepTime, value) : minStepTime;
            lastStepTime = lastStepTime == -1 && value >= 0 ? value : lastStepTime;
        }

        lastStepTime = Max(0, lastStepTime);

        // Update Times
        for (int value : engineStats.updateTimes)
        {
            maxUpdateTime = value >= 0 ? Max(maxUpdateTime, value) : maxUpdateTime;
            minUpdateTime = value >= 0 ? Min(minUpdateTime, value) : minUpdateTime;
            lastUpdateTime = lastUpdateTime == -1 && value >= 0 ? value : lastUpdateTime;
        }

        lastUpdateTime = Max(0, lastUpdateTime);

        // Render Times
        for (int value : engineStats.renderTimes)
        {
            maxRenderTime = value >= 0 ? Max(maxRenderTime, value) : maxRenderTime;
            minRenderTime = value >= 0 ? Min(minRenderTime, value) : minRenderTime;
            lastRenderTime = lastRenderTime == -1 && value >= 0 ? value : lastRenderTime;
        }

        lastRenderTime = Max(0, lastRenderTime);
    }

    // Draw Curves
    float fPositionX = 5.f;
    float fPositionY = 5.f;
    float fCurveWidth = Min(600.f, window->GetSize().x - 250.f);
    float fCurveHeight = 100.f;
    float fMarginTop = 10.f;

    sf::Color kColorCurveBox = sf::Color(255, 255, 255, 255);
    sf::Color kColorCurveFrameTimes = sf::Color(255, 255, 255, 255);
    sf::Color kColorCurveDrawCalls = sf::Color(255, 255, 70, 255);
    sf::Color colorCurveStepTimes = sf::Color(70, 200, 255, 255);
    sf::Color colorCurveUpdateTimes = sf::Color(255, 140, 50, 255);
    sf::Color colorCurveRenderTimes = sf::Color(255, 255, 255, 255);

    {
        GUGU_SCOPE_TRACE_MAIN("Curves");

        // Background
        Texture* textureBackground = GetResources()->GetCustomTexture("GuguConsoleBackground");
        m_statsBackground.setTexture(*textureBackground->GetSFTexture());
        m_statsBackground.setScale((fCurveWidth + fPositionX + 200.f) / textureBackground->GetSize().x, (fCurveHeight + fPositionY + 66.f) / textureBackground->GetSize().y);
        renderWindow->draw(m_statsBackground);

        // Step Times
        {
            int curveTopValue = Max(4, maxStepTime);
            float curvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float curvePointScaleY = (fCurveHeight - fMarginTop) / curveTopValue;

            int index = 0;
            sf::VertexArray curve(sf::PrimitiveType::Lines);
            curve.resize(engineStats.stepTimes.size() * 2);
            for (int value : engineStats.stepTimes)
            {
                value = Max(0, value);
                curve[index * 2] = (sf::Vertex(Vector2f(fPositionX + index * curvePointScaleX, fPositionY + fCurveHeight /* - value * curvePointScaleY*/), colorCurveStepTimes));
                curve[index * 2 + 1] = (sf::Vertex(Vector2f(fPositionX + index * curvePointScaleX, fPositionY + fCurveHeight - value * curvePointScaleY), colorCurveStepTimes));
                ++index;
            }

            renderWindow->draw(curve);
        }

        // Update Times
        {
            int curveTopValue = Max(4, maxUpdateTime);
            float fCurvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float fCurvePointScaleY = (fCurveHeight - fMarginTop) / curveTopValue;

            int iIndex = 0;
            sf::VertexArray kCurve(sf::PrimitiveType::LineStrip);
            kCurve.resize(engineStats.updateTimes.size());
            for (int iValue : engineStats.updateTimes)
            {
                kCurve[iIndex] = (sf::Vertex(Vector2f(fPositionX + iIndex * fCurvePointScaleX, fPositionY + fCurveHeight - iValue * fCurvePointScaleY), colorCurveUpdateTimes));
                ++iIndex;
            }

            renderWindow->draw(kCurve);
        }

        // Frame Times
        {
            int curveTopValue = Max(4, iMaxFrameTime);
            float fCurvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float fCurvePointScaleY = (fCurveHeight - fMarginTop) / curveTopValue;

            int iIndex = 0;
            sf::VertexArray kCurve(sf::PrimitiveType::LineStrip);
            kCurve.resize(m_statFrameTimes.size());
            for (int iValue : m_statFrameTimes)
            {
                kCurve[iIndex] = (sf::Vertex(Vector2f(fPositionX + iIndex * fCurvePointScaleX, fPositionY + fCurveHeight - iValue * fCurvePointScaleY), kColorCurveFrameTimes));
                ++iIndex;
            }

            renderWindow->draw(kCurve);
        }

        // Draw Calls
        {
            int curveTopValue = Max(4, iMaxDrawCalls);
            float fCurvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float fCurvePointScaleY = (fCurveHeight - fMarginTop) / curveTopValue;

            int iIndex = 0;
            sf::VertexArray kCurve(sf::PrimitiveType::LineStrip);
            kCurve.resize(m_statDrawCalls.size());
            for (int iValue : m_statDrawCalls)
            {
                kCurve[iIndex] = (sf::Vertex(Vector2f(fPositionX + iIndex * fCurvePointScaleX, fPositionY + fCurveHeight - iValue * fCurvePointScaleY), kColorCurveDrawCalls));
                ++iIndex;
            }

            renderWindow->draw(kCurve);
        }

        // Borders
        sf::VertexArray kBorder(sf::PrimitiveType::LineStrip);
        kBorder.resize(4);
        kBorder[0] = (sf::Vertex(Vector2f(fPositionX, fPositionY), kColorCurveBox));
        kBorder[1] = (sf::Vertex(Vector2f(fPositionX, fPositionY + fCurveHeight), kColorCurveBox));
        kBorder[2] = (sf::Vertex(Vector2f(fPositionX + fCurveWidth, fPositionY + fCurveHeight), kColorCurveBox));
        kBorder[3] = (sf::Vertex(Vector2f(fPositionX + fCurveWidth, fPositionY), kColorCurveBox));
        renderWindow->draw(kBorder);
    }

    // Draw Text Infos
    {
        GUGU_SCOPE_TRACE_MAIN("Text Infos");

        float fPosX = fPositionX + fCurveWidth + 10.f;
        float fPosY = fPositionY;
        float fGapY = 20.f;

        sf::Color kColorText = sf::Color(255, 255, 255, 255);

        // Frame Times
        m_statTextFameTime.setPosition(fPosX, fPosY);
        m_statTextFameTime.setFillColor(kColorText);
        m_statTextFameTime.setString(StringFormat("Frame: {0} ms", iFrameTime));
        renderWindow->draw(m_statTextFameTime);
        fPosY += fGapY;

        m_statTextFameTimeAverage.setPosition(fPosX, fPosY);
        m_statTextFameTimeAverage.setFillColor(kColorText);
        m_statTextFameTimeAverage.setString(StringFormat("Avg : {0} ms", fAvgFrameTime));
        renderWindow->draw(m_statTextFameTimeAverage);
        fPosY += fGapY;

        m_statTextFameTimeMin.setPosition(fPosX, fPosY);
        m_statTextFameTimeMin.setFillColor(kColorText);
        m_statTextFameTimeMin.setString(StringFormat("Min: {0} ms", iMinFrameTime));
        renderWindow->draw(m_statTextFameTimeMin);
        fPosY += fGapY;

        m_statTextFameTimeMax.setPosition(fPosX, fPosY);
        m_statTextFameTimeMax.setFillColor(kColorText);
        m_statTextFameTimeMax.setString(StringFormat("Max: {0} ms", iMaxFrameTime));
        renderWindow->draw(m_statTextFameTimeMax);
        fPosY += fGapY;

        m_statTextFPS.setPosition(fPosX, fPosY);
        m_statTextFPS.setFillColor(kColorText);
        m_statTextFPS.setString(StringFormat("FPS: {0}", iFPS));
        renderWindow->draw(m_statTextFPS);
        fPosY += fGapY;

        // Step Times
        m_statTextStepTime.setPosition(fPositionX, fPositionY + fCurveHeight);
        m_statTextStepTime.setFillColor(colorCurveStepTimes);
        m_statTextStepTime.setString(StringFormat("Step: {0} ms (min: {1} ms, max: {2} ms)", lastStepTime, minStepTime, maxStepTime));
        renderWindow->draw(m_statTextStepTime);

        // Update Times
        m_statTextUpdateTime.setPosition(fPositionX, fPositionY + 22.f + fCurveHeight);
        m_statTextUpdateTime.setFillColor(colorCurveUpdateTimes);
        m_statTextUpdateTime.setString(StringFormat("Update: {0} ms (min: {1} ms, max: {2} ms)", lastUpdateTime, minUpdateTime, maxUpdateTime));
        renderWindow->draw(m_statTextUpdateTime);

        // Render Times
        m_statTextRenderTime.setPosition(fPositionX, fPositionY + 44.f + fCurveHeight);
        m_statTextRenderTime.setFillColor(colorCurveRenderTimes);
        m_statTextRenderTime.setString(StringFormat("Render: {0} ms (min: {1} ms, max: {2} ms)", lastRenderTime, minRenderTime, maxRenderTime));
        renderWindow->draw(m_statTextRenderTime);

        // Draw Calls
        m_statTextDrawCalls.setPosition(fPositionX + 320.f, fPositionY + fCurveHeight);
        m_statTextDrawCalls.setFillColor(kColorCurveDrawCalls);
        m_statTextDrawCalls.setString(StringFormat("Draw calls: {0} (tris: {1})", kFrameInfos.statDrawCalls, kFrameInfos.statTriangles));
        renderWindow->draw(m_statTextDrawCalls);

        // Is Tracing
        if (engineStats.isTracing)
        {
            m_statTextIsTracing.setPosition(fPosX, fPositionY + fCurveHeight);
            renderWindow->draw(m_statTextIsTracing);
        }
    }
}

}   // namespace gugu
