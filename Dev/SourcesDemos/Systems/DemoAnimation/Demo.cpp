////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/System/Memory.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
    : m_root(nullptr)
    , m_walkStepToggle(false)
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

    float margin = 10.f;
    float cellSize = 200.f;
    int nbCells = 6;
    float lineSize = margin * 2 + (cellSize * (nbCells - 1));
    for (int x = 0; x < nbCells; ++x)
    {
        gridVertices->append(sf::Vertex(Vector2f(0.f, margin + x * cellSize)));
        gridVertices->append(sf::Vertex(Vector2f(lineSize, margin + x * cellSize)));

        gridVertices->append(sf::Vertex(Vector2f(margin + x * cellSize, 0.f)));
        gridVertices->append(sf::Vertex(Vector2f(margin + x * cellSize, lineSize)));
    }

    ElementSFDrawable* gridElement = m_root->AddChild<ElementSFDrawable>();
    gridElement->SetSFDrawable(gridVertices);

    float lineA = margin;
    float lineB = margin + cellSize * 1;
    float lineC = margin + cellSize * 2;
    float lineD = margin + cellSize * 3;
    float lineE = margin + cellSize * 4;


    // Test Animation Idle 1 (no origin set).
    ElementSprite* spriteIdle1 = m_root->AddChild<ElementSprite>();
    spriteIdle1->SetPosition(lineA, lineA);
    spriteIdle1->SetScale(0.5f);

    SpriteAnimation* animationIdle1 = GetAnimations()->AddAnimation(spriteIdle1);
    animationIdle1->ChangeAnimSet("Dinosaur.animset.xml");
    animationIdle1->StartAnimation("Idle");

    // Test Animation Idle 2 (no origin set) with a flipped texture.
    ElementSprite* spriteIdle2 = m_root->AddChild<ElementSprite>();
    spriteIdle2->SetPosition(lineB, lineA);
    spriteIdle2->SetScale(0.5f);
    spriteIdle2->SetFlipTextureH(true);

    SpriteAnimation* animationIdle2 = GetAnimations()->AddAnimation(spriteIdle2);
    animationIdle2->ChangeAnimSet("Dinosaur.animset.xml");
    animationIdle2->StartAnimation("Idle");

    // Test Animation Idle 3 (with flip).
    ElementSprite* spriteIdle3 = m_root->AddChild<ElementSprite>();
    spriteIdle3->SetPosition(lineC, lineA);
    spriteIdle3->SetScale(0.5f);
    spriteIdle3->SetFlipH(true);

    SpriteAnimation* animationIdle3 = GetAnimations()->AddAnimation(spriteIdle3);
    animationIdle3->ChangeAnimSet("Dinosaur.animset.xml");
    animationIdle3->StartAnimation("Idle");

    // Test Animation Idle 4 (with flip) with a flipped texture.
    ElementSprite* spriteIdle4 = m_root->AddChild<ElementSprite>();
    spriteIdle4->SetPosition(lineD, lineA);
    spriteIdle4->SetScale(0.5f);
    spriteIdle4->SetFlipTextureH(true);
    spriteIdle4->SetFlipH(true);

    SpriteAnimation* animationIdle4 = GetAnimations()->AddAnimation(spriteIdle4);
    animationIdle4->ChangeAnimSet("Dinosaur.animset.xml");
    animationIdle4->StartAnimation("Idle");

    // Test Animation Walk (no origin set : top left).
    ElementSprite* spriteWalk1 = m_root->AddChild<ElementSprite>();
    spriteWalk1->SetPosition(lineA, lineB);
    spriteWalk1->SetScale(0.5f);

    SpriteAnimation* animationWalk1 = GetAnimations()->AddAnimation(spriteWalk1);
    animationWalk1->ChangeAnimSet("Dinosaur.animset.xml");
    animationWalk1->StartAnimation("Walk");

    // Test Animation Walk (origin set manually : bottom center).
    ElementSprite* aspriteWalk2 = m_root->AddChild<ElementSprite>();
    aspriteWalk2->SetPosition(lineB, lineB);
    aspriteWalk2->SetScale(0.5f);
    aspriteWalk2->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);

    SpriteAnimation* animationWalk2 = GetAnimations()->AddAnimation(aspriteWalk2);
    animationWalk2->ChangeAnimSet("Dinosaur.animset.xml");
    animationWalk2->StartAnimation("Walk");

    // Test Animation Walk (origin set in animation).
    ElementSprite* spriteWalk3 = m_root->AddChild<ElementSprite>();
    spriteWalk3->SetPosition(lineC, lineB);
    spriteWalk3->SetScale(0.5f);

    SpriteAnimation* animationWalk3 = GetAnimations()->AddAnimation(spriteWalk3);
    animationWalk3->SetOriginFromAnimation(true);
    animationWalk3->ChangeAnimSet("Dinosaur.animset.xml");
    animationWalk3->StartAnimation("Walk");

    // Test Animation Events (no origin set : top left).
    ElementSprite* spriteWalk4 = m_root->AddChild<ElementSprite>();
    spriteWalk4->SetPosition(lineD, lineB);
    spriteWalk4->SetScale(0.5f);

    SpriteAnimation* animationWalk4 = GetAnimations()->AddAnimation(spriteWalk4);
    animationWalk4->ChangeAnimSet("Dinosaur.animset.xml");
    animationWalk4->StartAnimation("Walk");
    animationWalk4->AddEventCallback("step", std::bind(&Demo::OnWalkStep, this));

    Element* walkStepTextPivot = m_root->AddChild<Element>();
    walkStepTextPivot->SetPosition(lineD + 30.f, lineB + 90.f);

    m_walkStepText = walkStepTextPivot->AddChild<ElementText>();
    m_walkStepText->SetText("step");

    // Test Animation Walk (origin and move set in animation).
    m_movingSprite1 = m_root->AddChild<ElementSprite>();
    m_movingSprite1->SetPosition(lineD, lineC);
    m_movingSprite1->SetScale(0.5f);
    m_movingSprite1->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);

    m_movingAnimation1 = GetAnimations()->AddAnimation(m_movingSprite1);
    m_movingAnimation1->SetMoveFromAnimation(true);
    m_movingAnimation1->ChangeAnimSet("Dinosaur.animset.xml");
    m_movingAnimation1->StartAnimation("Walk");

    // Test fake shadow (reversed animation).
    m_movingSprite2 = m_root->AddChild<ElementSprite>();
    m_movingSprite2->SetPosition(lineD, lineC);
    m_movingSprite2->SetScale(0.5f, 0.25f);
    m_movingSprite2->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    m_movingSprite2->SetFlipV(true);
    m_movingSprite2->SetColor(sf::Color(80, 80, 80, 128));

    m_movingAnimation2 = GetAnimations()->AddAnimation(m_movingSprite2);
    m_movingAnimation2->SetMoveFromAnimation(true);
    m_movingAnimation2->ChangeAnimSet("Dinosaur.animset.xml");
    m_movingAnimation2->StartAnimation("Walk");
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
    
    if (m_movingSprite1->GetPosition().x <= lineB)
    {
        m_movingSprite1->SetFlipH(true);
        m_movingSprite2->SetFlipH(true);
    }
    else if (m_movingSprite1->GetPosition().x >= lineE)
    {
        m_movingSprite1->SetFlipH(false);
        m_movingSprite2->SetFlipH(false);
    }
}

void Demo::OnWalkStep()
{
    m_walkStepText->SetPositionX(m_walkStepToggle ? 25.f : -25.f);
    m_walkStepToggle = !m_walkStepToggle;
}

}   //namespace demoproject
