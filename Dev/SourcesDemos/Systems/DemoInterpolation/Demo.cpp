////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Window/Vertex2.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/System/Memory.h"
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/CircleShape.hpp>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
: m_root(nullptr)
{
}

Demo::~Demo()
{
}

void Demo::AppStart()
{
    RegisterEventHandler(GetGameWindow());

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);

    // Grid
    sf::VertexArray* gridVertices = new sf::VertexArray;
    gridVertices->setPrimitiveType(sf::PrimitiveType::Lines);

    float margin = 150.f;
    float columnStep = 300.f;
    float columnSize = 180.f;
    float lineOffset = 50.f;

    int nbCells = 3;
    float lineSize = margin * 2 + (columnStep * (nbCells));
    for (int x = 0; x < nbCells; ++x)
    {
        AppendLineVertices(*gridVertices
            , Vector2f(margin + x * columnStep, 0.f)
            , Vector2f(margin + x * columnStep, lineSize));
        AppendLineVertices(*gridVertices
            , Vector2f(margin + columnSize + x * columnStep, 0.f)
            , Vector2f(margin + columnSize + x * columnStep, lineSize));
    }

    ElementSFDrawable* gridElement = m_root->AddChild<ElementSFDrawable>();
    gridElement->SetSFDrawable(gridVertices);

    // Titles
    float positionX = 5.f;
    float positionY = 50.f;
    ElementText* title;

    title = m_root->AddChild<ElementText>();
    title->SetText("Linear");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Quad");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Cubic");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Quart");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Quint");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Sine");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Expo");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Circ");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Back");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Elastic");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    positionY += lineOffset;
    title = m_root->AddChild<ElementText>();
    title->SetText("Bounce");
    title->SetUnifiedOrigin(UDim2::POSITION_CENTER_LEFT);
    title->SetPosition(positionX, positionY);

    // Dots
    for (int i = 0; i < 33; ++i)
    {
        sf::CircleShape* shape = new sf::CircleShape(5.f);
        shape->setFillColor(sf::Color::Black);

        ElementSFDrawable* point = m_root->AddChild<ElementSFDrawable>();
        point->SetSFDrawable(shape);
        point->SetOrigin(5.f, 5.f);

        points.push_back(point);
    }

    m_animationTimeMs = 0;
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    float margin = 150.f;
    float columnStep = 300.f;
    float columnSize = 180.f;
    float lineOffset = 50.f;

    int interpolationDurationMs = 1000;
    int cycleDurationMs = 1500;

    float positionMin = margin;
    float positionMax = margin + columnSize;

    m_animationTimeMs += static_cast<int>(dt.ms());
    m_animationTimeMs %= cycleDurationMs;

    float linearRatio = (m_animationTimeMs > interpolationDurationMs) ? 1.f : (float)m_animationTimeMs / (float)interpolationDurationMs;

    int i = 0;
    float positionY = 50.f;

    points[i++]->SetPosition(Lerp(positionMin, positionMax, linearRatio), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, linearRatio) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, linearRatio) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInQuad(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutQuad(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutQuad(linearRatio)) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInCubic(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutCubic(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutCubic(linearRatio)) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInQuart(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutQuart(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutQuart(linearRatio)) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInQuint(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutQuint(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutQuint(linearRatio)) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInSine(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutSine(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutSine(linearRatio)) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInExpo(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutExpo(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutExpo(linearRatio)) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInCirc(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutCirc(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutCirc(linearRatio)) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInBack(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutBack(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutBack(linearRatio)) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInElastic(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutElastic(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutElastic(linearRatio)) + columnStep * 2, positionY);

    positionY += lineOffset;
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInBounce(linearRatio)), positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseOutBounce(linearRatio)) + columnStep, positionY);
    points[i++]->SetPosition(Lerp(positionMin, positionMax, EaseInOutBounce(linearRatio)) + columnStep * 2, positionY);
}

}   //namespace demoproject
