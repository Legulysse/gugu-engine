////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/Utility/System.h"

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
    RegisterHandlerEvents(GetGameWindow());

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    // Grid
    sf::VertexArray* gridVertices = new sf::VertexArray;
    gridVertices->setPrimitiveType(sf::PrimitiveType::Lines);

    float margin = 10.f;
    float cellSize = 200.f;
    int nbCells = 6;
    float lineSize = margin * 2 + (cellSize * (nbCells - 1));
    for (int x = 0; x < nbCells; ++x)
    {
        gridVertices->append(sf::Vertex(sf::Vector2f(0.f, margin + x * cellSize)));
        gridVertices->append(sf::Vertex(sf::Vector2f(lineSize, margin + x * cellSize)));

        gridVertices->append(sf::Vertex(sf::Vector2f(margin + x * cellSize, 0.f)));
        gridVertices->append(sf::Vertex(sf::Vector2f(margin + x * cellSize, lineSize)));
    }

    ElementSFDrawable* gridElement = m_root->AddChild<ElementSFDrawable>();
    gridElement->SetSFDrawable(gridVertices);

    float lineA = margin;
    float lineB = margin + cellSize * 1;
    float lineC = margin + cellSize * 2;
    float lineD = margin + cellSize * 3;
    float lineE = margin + cellSize * 4;


    // Test Animation Idle (no origin set)
    ElementSpriteAnimated* animationIdle = m_root->AddChild<ElementSpriteAnimated>();
    animationIdle->SetPosition(lineA, lineB);
    animationIdle->SetScale(0.5f);
    animationIdle->ChangeAnimSet("Dinosaur.animset.xml");
    animationIdle->StartAnimation("Idle");
    animationIdle->SetFlipTextureY(true);
    //animationIdle->AddEventCallback("Footstep", new ActionStatic(PlayFootStep)); //TODO: optionnal event demo (like a button to trigger the walk, or an archer shooting arrows)


    // Test Animation Walk (no origin set : top left)
    ElementSpriteAnimated* animationWalk1 = m_root->AddChild<ElementSpriteAnimated>();
    animationWalk1->SetPosition(lineB, lineB);
    animationWalk1->SetScale(0.5f);
    animationWalk1->ChangeAnimSet("Dinosaur.animset.xml");
    animationWalk1->StartAnimation("Walk");

    // Test Animation Walk (origin set manually : bottom center)
    ElementSpriteAnimated* animationWalk2 = m_root->AddChild<ElementSpriteAnimated>();
    animationWalk2->SetPosition(lineC, lineB);
    animationWalk2->SetScale(0.5f);
    animationWalk2->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    animationWalk2->ChangeAnimSet("Dinosaur.animset.xml");
    animationWalk2->StartAnimation("Walk");

    // Test Animation Walk (origin set in animation)
    ElementSpriteAnimated* animationWalk3 = m_root->AddChild<ElementSpriteAnimated>();
    animationWalk3->SetPosition(lineD, lineB);
    animationWalk3->SetScale(0.5f);
    animationWalk3->SetOriginFromAnimation(true);
    animationWalk3->ChangeAnimSet("Dinosaur.animset.xml");
    animationWalk3->StartAnimation("Walk");


    // Test Animation Walk (origin and move set in animation)
    m_movingAnimationA = m_root->AddChild<ElementSpriteAnimated>();
    m_movingAnimationA->SetPosition(lineD, lineD);
    m_movingAnimationA->SetScale(0.5f);
    m_movingAnimationA->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    //m_movingAnimationA->SetOriginFromAnimation(true);
    m_movingAnimationA->SetMoveFromAnimation(true);
    m_movingAnimationA->ChangeAnimSet("Dinosaur.animset.xml");
    m_movingAnimationA->StartAnimation("Walk");

    // Test fake shadow (reversed animation)
    m_movingAnimationB = m_root->AddChild<ElementSpriteAnimated>();
    m_movingAnimationB->SetPosition(lineD, lineD);
    m_movingAnimationB->SetScale(0.5f, 0.25f);
    m_movingAnimationB->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    //m_movingAnimationB->SetOriginFromAnimation(true);
    m_movingAnimationB->SetMoveFromAnimation(true);
    m_movingAnimationB->ChangeAnimSet("Dinosaur.animset.xml");
    m_movingAnimationB->StartAnimation("Walk");
    m_movingAnimationB->SetFlipX(true);
    m_movingAnimationB->SetColor(sf::Color(80, 80, 80, 128));
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::AppStep(const DeltaTime& dt)
{
    float margin = 50.f;
    float cellSize = 200.f;
    float lineA = margin;
    float lineB = margin + cellSize * 1;
    float lineC = margin + cellSize * 2;
    float lineD = margin + cellSize * 3;
    float lineE = margin + cellSize * 4;

    if (m_movingAnimationA->GetPosition().x <= lineB)
    {
        m_movingAnimationA->SetFlipY(true);
        m_movingAnimationB->SetFlipY(true);
    }
    else if (m_movingAnimationA->GetPosition().x >= lineE)
    {
        m_movingAnimationA->SetFlipY(false);
        m_movingAnimationB->SetFlipY(false);
    }
}

}   //namespace demoproject
