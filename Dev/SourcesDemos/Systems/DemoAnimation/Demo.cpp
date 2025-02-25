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

    // Metrics
    float animationColumnA = 300.f;
    float animationColumnB = 800.f;
    float lineTopA = 20.f;
    float lineTopB = 150.f;
    float lineOffset = 150.f;
    UDim2 detailsPositionOffsetA = UDim2::POSITION_TOP_LEFT + Vector2f(290.f, 0.f);
    UDim2 detailsPositionOffsetB = UDim2::POSITION_BOTTOM_LEFT + Vector2f(290.f, 0.f);

    // Grid
    sf::VertexArray* gridVertices = new sf::VertexArray;
    gridVertices->setPrimitiveType(sf::PrimitiveType::Lines);

    gridVertices->append(sf::Vertex(Vector2f(animationColumnA, 0.f)));
    gridVertices->append(sf::Vertex(Vector2f(animationColumnA, 1000.f)));
    gridVertices->append(sf::Vertex(Vector2f(animationColumnB, 0.f)));
    gridVertices->append(sf::Vertex(Vector2f(animationColumnB, 1000.f)));

    for (int i = 0; i < 4; ++i)
    {
        Vector2f position = Vector2f(animationColumnA, lineTopA + lineOffset * i);
        gridVertices->append(sf::Vertex(position + Vector2f(-20.f, 0.f)));
        gridVertices->append(sf::Vertex(position + Vector2f(20.f, 0.f)));
    }

    for (int i = 0; i < 4; ++i)
    {
        Vector2f position = Vector2f(animationColumnB, lineTopB + lineOffset * i);
        gridVertices->append(sf::Vertex(position + Vector2f(-20.f, 0.f)));
        gridVertices->append(sf::Vertex(position + Vector2f(20.f, 0.f)));
    }

    ElementSFDrawable* gridElement = m_root->AddChild<ElementSFDrawable>();
    gridElement->SetSFDrawable(gridVertices);

    // Test Animation Idle (no origin set).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(animationColumnA, lineTopA + lineOffset * 0);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Idle");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Default");
        details->SetUnifiedOrigin(detailsPositionOffsetA);
    }

    // Test Animation Idle (with flip).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(animationColumnA, lineTopA + lineOffset * 1);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetFlipH(true);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Idle");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Pivot flip");
        details->SetUnifiedOrigin(detailsPositionOffsetA);
    }

    // Test Animation Idle (no origin set) with a flipped texture.
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(animationColumnA, lineTopA + lineOffset * 2);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetFlipTextureH(true);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Idle");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Texture flip");
        details->SetUnifiedOrigin(detailsPositionOffsetA);
    }

    // Test Animation Idle (with flip) with a flipped texture.
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(animationColumnA, lineTopA + lineOffset * 3);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetFlipTextureH(true);
        sprite->SetFlipH(true);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Idle");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Pivot flip, Texture flip");
        details->SetUnifiedOrigin(detailsPositionOffsetA);
    }

    // Test Animation Walk (origin set manually : bottom center).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(animationColumnB, lineTopB + lineOffset * 0);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->SetOriginFromAnimation(false);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Walk");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Pivot: BottomCenter");
        details->SetUnifiedOrigin(detailsPositionOffsetB);
    }

    // Test Animation Walk (origin set in animation).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(animationColumnB, lineTopB + lineOffset * 1);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->SetOriginFromAnimation(true);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Walk");

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Pivot: BottomCenter\nOriginFromAnimation");
        details->SetUnifiedOrigin(detailsPositionOffsetB);
    }

    // Test Animation Events (no origin set : top left).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(animationColumnB, lineTopB + lineOffset * 2);

        ElementSprite* sprite = pivot->AddChild<ElementSprite>();
        sprite->SetScale(0.5f);
        sprite->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);

        SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
        animation->ChangeAnimSet("Dinosaur.animset.xml");
        animation->StartAnimation("Walk");
        animation->AddEventCallback("step", std::bind(&Demo::OnWalkStep, this));

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Pivot: BottomCenter\nEvents");
        details->SetUnifiedOrigin(detailsPositionOffsetB);

        Element* walkStepTextPivot = pivot->AddChild<Element>();

        m_walkStepText = walkStepTextPivot->AddChild<ElementText>();
        m_walkStepText->SetText("step");
    }

    // Test Animation Walk (origin and move set in animation).
    {
        Element* pivot = m_root->AddChild<Element>();
        pivot->SetPosition(animationColumnB, lineTopB + lineOffset * 3);

        ElementText* details = pivot->AddChild<ElementText>();
        details->SetText("Pivot: BottomCenter\nMoveFromAnimation");
        details->SetUnifiedOrigin(detailsPositionOffsetB);

        m_movingSprite1 = pivot->AddChild<ElementSprite>();
        m_movingSprite1->SetScale(0.5f);
        m_movingSprite1->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);

        m_movingAnimation1 = GetAnimations()->AddAnimation(m_movingSprite1);
        m_movingAnimation1->SetMoveFromAnimation(true);
        m_movingAnimation1->ChangeAnimSet("Dinosaur.animset.xml");
        m_movingAnimation1->StartAnimation("Walk");

        // Test fake shadow (reversed animation) (I am not linking the two sprites directly to test that moving a flipped sprite works as intended).
        m_movingSprite2 = pivot->AddChild<ElementSprite>();
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
    float animationColumnA = 300.f;
    float animationColumnB = 800.f;

    if (m_movingSprite1->TransformToGlobal(m_movingSprite1->GetPosition()).x <= animationColumnA)
    {
        m_movingSprite1->SetFlipH(true);
        m_movingSprite2->SetFlipH(true);
    }
    else if (m_movingSprite1->TransformToGlobal(m_movingSprite1->GetPosition()).x >= animationColumnB)
    {
        m_movingSprite1->SetFlipH(false);
        m_movingSprite2->SetFlipH(false);
    }
}

void Demo::OnWalkStep()
{
    m_walkStepText->SetPositionX(m_walkStepToggle ? 15.f : -35.f);
    m_walkStepToggle = !m_walkStepToggle;
}

}   //namespace demoproject
