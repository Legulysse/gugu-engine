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
    , m_speedFactor(1.f)
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

    if (ImGui::SliderFloat("Speed Factor", &m_speedFactor, 0.1f, 10.f))
    {
        m_spriteAnimation->SetAnimationSpeed(m_speedFactor);
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
#if 1
    //const ImGuiStyle& style = ImGui::GetStyle();

    //bool changedSelectionThisFrame = false;

    //ImGuiTreeNodeFlags animationNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
    //ImGuiTreeNodeFlags frameNodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

    // Animations list.
    ImGuiTableFlags animationTableflags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY /*| ImGuiTableFlags_NoPadInnerX */;
    if (ImGui::BeginTable("_ANIMATIONS_TABLE", 3, animationTableflags))
    {
        ImGuiTableColumnFlags animationColumnFlags = ImGuiTableColumnFlags_WidthFixed;
        ImGui::TableSetupColumn("name", animationColumnFlags, 100.f);
        ImGui::TableSetupColumn("play", animationColumnFlags, 30.f);
        ImGui::TableSetupColumn("duration", animationColumnFlags, 60.f);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        const std::vector<Animation*>& animations = m_animSet->GetAnimations();

        for (size_t animationIndex = 0; animationIndex < animations.size(); ++animationIndex)
        {
            ImGui::PushID(animationIndex);

            float row_min_height = ImGui::GetFrameHeight();
            ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

            if (animationIndex == 0)
            {
                // Setup ItemWidth once.
                int headerIndex = 0;

                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
            }

            Animation* animation = animations[animationIndex];

            int columnIndex = 0;
            ImGui::TableSetColumnIndex(columnIndex++);
            //bool isAnimationNodeOpen = ImGui::TreeNodeEx("##_ANIMATION_TREENODE", animationNodeFlags);

            //bool wasAnimationNodeOpen = StdSetContains(m_openedAnimations, animation->GetName());
            //bool isAnimationNodeOpen = wasAnimationNodeOpen;
            bool isAnimationNodeOpen = true;

            //ImGui::SameLine();
            //ImGui::TableSetColumnIndex(columnIndex++);
            ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns /*| ImGuiSelectableFlags_AllowItemOverlap*/;
            if (ImGui::Selectable(animation->GetName().c_str(), m_currentAnimation == animation, selectable_flags, ImVec2(0, row_min_height)))
            {
                if (m_currentAnimation != animation)
                {
                    m_currentAnimation = animation;
                    m_spriteAnimation->StartAnimation(animation->GetName());

                    m_currentFrame = nullptr;
                    if (m_currentAnimation->GetFrameCount() > 0)
                    {
                        m_currentFrame = m_currentAnimation->GetFrame(0);
                    }
                }
            }

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text(m_spriteAnimation->GetAnimation() == animation ? "<--" : "");

            // Empty column.
            ImGui::TableSetColumnIndex(columnIndex++);

            //ImGui::TableSetColumnIndex(columnIndex++);
            //ImGui::Text(animation->GetName().c_str());

            //if (m_currentAnimation == animations[animationIndex] && !changedSelectionThisFrame)
            if (isAnimationNodeOpen)
            {
                ImGui::Indent();

                // Frames list.
             //   ImGuiTableFlags frameTableflags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable /*| ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoPadInnerX */;
             //   if (ImGui::BeginTable("Frames Table", 2, frameTableflags))
                {
                   // ImGuiTableColumnFlags frameColumnFlags = ImGuiTableColumnFlags_WidthFixed;
                   // ImGui::TableSetupColumn("#", frameColumnFlags, 30.f);
                   // ImGui::TableSetupColumn("duration", frameColumnFlags, 150.f);
                   //// ImGui::TableSetupScrollFreeze(0, 1);
                   // ImGui::TableHeadersRow();

                    const std::vector<AnimationFrame*>& animationFrames = animation->GetFrames();

                    for (size_t frameIndex = 0; frameIndex < animationFrames.size(); ++frameIndex)
                    {
                        ImGui::PushID(frameIndex);

                        float frame_row_min_height = ImGui::GetFrameHeight();
                        ImGui::TableNextRow(ImGuiTableRowFlags_None, frame_row_min_height);

                        //if (frameIndex == 0)
                        //{
                        //    // Setup ItemWidth once.
                        //    int headerIndex = 0;

                        //    ImGui::TableSetColumnIndex(headerIndex++);
                        //    ImGui::PushItemWidth(-1);
                        //    ImGui::TableSetColumnIndex(headerIndex++);
                        //    ImGui::PushItemWidth(-1);
                        //}

                        int frameColumnIndex = 0;
                        ImGui::TableSetColumnIndex(frameColumnIndex++);

                        std::string frameName = StringFormat("{0} [{1}]", animation->GetName(), frameIndex);

                        ImGuiSelectableFlags selectable_frame_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
                        if (ImGui::Selectable(frameName.c_str(), m_currentFrame == animationFrames[frameIndex], selectable_frame_flags, ImVec2(0, frame_row_min_height)))
                        {
                            if (m_currentAnimation != animation)
                            {
                                m_currentAnimation = animation;
                                m_spriteAnimation->StartAnimation(m_currentAnimation->GetName());
                            }

                            m_currentFrame = animationFrames[frameIndex];

                            if (!m_autoPlay)
                            {
                                m_spriteAnimation->SetCurrentFrame(frameIndex);
                            }
                        }

                        ImGui::TableSetColumnIndex(frameColumnIndex++);
                        ImGui::Text(m_spriteAnimation->GetAnimationFrame() == animationFrames[frameIndex] ? "<-" : "");

                        //ImGui::TableSetColumnIndex(frameColumnIndex++);
                        //ImGui::Text("%f", animationFrames[frameIndex]->GetDuration());

                        ImGui::TableSetColumnIndex(frameColumnIndex++);
                        float frameDuration = animationFrames[frameIndex]->GetDuration();
                        if (ImGui::InputFloat("##_FRAME_DURATION", &frameDuration))
                        {
                            animationFrames[frameIndex]->SetDuration(frameDuration);
                            RaiseDirty();
                        }

                        ImGui::PopID();
                    }

                    ImGui::Unindent();
                //    ImGui::EndTable();
                }
            }

            //ImGui::TableSetColumnIndex(columnIndex++);
            //ImGui::Text(animation->GetName().c_str());

            if (isAnimationNodeOpen)
            {
                //ImGui::TreePop();
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
#else
    {
        const std::vector<Animation*>& animations = m_animSet->GetAnimations();

        for (size_t i = 0; i < animations.size(); ++i)
        {
            if (ImGui::Button(animations[i]->GetName().c_str()))
            {
                m_spriteAnimation->StartAnimation(animations[i]->GetName());

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
#endif
}

}   //namespace gugu
