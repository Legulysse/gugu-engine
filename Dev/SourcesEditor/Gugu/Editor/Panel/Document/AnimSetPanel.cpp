////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/AnimSetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AnimSetPanel::AnimSetPanel(AnimSet* resource)
    : DocumentPanel(resource)
    , m_animSet(resource)
    , m_renderViewport(nullptr)
    , m_zoomFactor(1.f)
    , m_autoPlay(true)
    , m_spriteAnimation(nullptr)
    , m_currentAnimation(nullptr)
    , m_currentFrame(nullptr)
{
    // Setup RenderViewport and Sprite.
    m_renderViewport = new RenderViewport(true);
    m_renderViewport->SetSize(Vector2u(500, 500));

    // Setup animation
    ElementSprite* sprite = m_renderViewport->GetRoot()->AddChild<ElementSprite>();
    sprite->SetUnifiedPosition(UDim2::POSITION_CENTER);
    sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);

    m_spriteAnimation = GetAnimations()->AddAnimation(sprite);
    m_spriteAnimation->ChangeAnimSet(m_animSet);
}

AnimSetPanel::~AnimSetPanel()
{
    SafeDelete(m_renderViewport);
}

void AnimSetPanel::OnUndoRedo()
{
    // TODO: keep animation and frame names, and try to restore selection.
    m_currentAnimation = nullptr;
    m_currentFrame = nullptr;

    m_spriteAnimation->StopAnimation();
}

void AnimSetPanel::UpdatePanelImpl(const DeltaTime& dt)
{
    // Toolbar.
    if (ImGui::SliderFloat("Zoom Factor", &m_zoomFactor, 1.f, 16.f))
    {
        m_renderViewport->SetZoom(m_zoomFactor);
    }

    if (ImGui::Checkbox("Play", &m_autoPlay))
    {
        m_spriteAnimation->SetAnimationPause(!m_autoPlay);
    }

    // Viewport.
    m_renderViewport->ImGuiBegin();
    m_renderViewport->ImGuiEnd();
}

void AnimSetPanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
    const std::vector<Animation*>& animations = m_animSet->GetAnimations();

    for (size_t i = 0; i < animations.size(); ++i)
    {
        if (ImGui::Button(animations[i]->GetName().c_str()))
        {
            m_spriteAnimation->StartAnimation(animations[i]->GetName());
            m_spriteAnimation->SetAnimationPause(!m_autoPlay);

            m_currentAnimation = animations[i];
            m_currentFrame = nullptr;
        }

        if (m_currentAnimation == animations[i])
        {
            ImGui::SameLine();
            ImGui::Text("Selected");
        }
    }

    if (m_currentAnimation)
    {
        const std::vector<AnimationFrame*>& animationFrames = m_currentAnimation->GetFrames();

        for (size_t i = 0; i < animationFrames.size(); ++i)
        {
            if (ImGui::Button(StringFormat("Frame {0}", i).c_str()))
            {
                if (!m_autoPlay)
                {
                    m_spriteAnimation->SetCurrentFrame(i);
                }

                m_currentFrame = animationFrames[i];
            }

            if (m_currentFrame == animationFrames[i])
            {
                ImGui::SameLine();
                ImGui::Text("Selected");
            }
        }
    }

    if (m_currentFrame)
    {
        float frameDuration = m_currentFrame->GetDuration();
        if (ImGui::InputFloat("Duration", &frameDuration))
        {
            m_currentFrame->SetDuration(frameDuration);
            RaiseDirty();
        }
    }
}

}   //namespace gugu
