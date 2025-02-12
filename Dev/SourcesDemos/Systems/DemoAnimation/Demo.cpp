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
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(lineA, lineA);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Idle");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Default");
        details->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    }

    // Test Animation Idle 2 (no origin set) with a flipped texture.
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(lineB, lineA);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetFlipTextureH(true);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Idle");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Texture flip");
        details->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    }

    // Test Animation Idle 3 (with flip).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(lineC, lineA);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetFlipH(true);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Idle");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Pivot flip");
        details->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    }

    // Test Animation Idle 4 (with flip) with a flipped texture.
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(lineD, lineA);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetFlipTextureH(true);
        sprite->SetFlipH(true);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Idle");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Pivot flip, Texture flip");
        details->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    }

    // Test Animation Walk (no origin set : top left).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(lineA, lineB);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Walk");
    }

    // Test Animation Walk (origin set manually : bottom center).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(lineB, lineB);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Walk");
    }

    // Test Animation Walk (origin set in animation).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(lineC, lineB);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->SetOriginFromAnimation(true);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Walk");
    }

    // Test Animation Events (no origin set : top left).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(lineD, lineB);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Walk");
        animation->AddEventCallback("step", std::bind(&Demo::OnWalkStep, this));

        Element* walkStepTextPivot = pivot->AddChild<Element>();
        walkStepTextPivot->SetPosition(30.f, 90.f);

        m_walkStepText = walkStepTextPivot->AddChild<ElementText>();
        m_walkStepText->SetText("step");
    }

    // Test Animation Walk (origin and move set in animation).
    {
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
