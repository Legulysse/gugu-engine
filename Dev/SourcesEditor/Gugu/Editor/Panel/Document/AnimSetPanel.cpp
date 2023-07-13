////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/AnimSetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Core/ProjectSettings.h"
#include "Gugu/Editor/Widget/RenderViewport.h"
#include "Gugu/Editor/Modal/GenerateAnimationFramesDialog.h"

#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AnimSetPanel::AnimSetPanel(AnimSet* resource)
    : DocumentPanel(resource)
    , m_animSet(resource)
    , m_renderViewport(nullptr)
    , m_zoomFactor(1.f)
    , m_speedFactor(1.f)
    , m_autoPlay(true)
    , m_originFromAnimation(false)
    , m_moveFromAnimation(false)
    , m_flipH(false)
    , m_currentAnimation(nullptr)
    , m_currentFrame(nullptr)
    , m_defaultDuration(0.1f)
    , m_spriteAnimation(nullptr)
    , m_sprite(nullptr)
{
    // Setup RenderViewport and Sprite.
    m_renderViewport = new RenderViewport(true);
    m_renderViewport->SetSize(Vector2u(500, 500));

    // Setup animation
    m_sprite = m_renderViewport->GetRoot()->AddChild<ElementSprite>();
    m_sprite->SetUnifiedPosition(UDim2::POSITION_CENTER);
    m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);

    m_spriteAnimation = GetAnimations()->AddAnimation(m_sprite);
    m_spriteAnimation->ChangeAnimSet(m_animSet);

    SelectAnimation(m_animSet->GetAnimation(0));

    // Dependencies
    GetResources()->RegisterResourceListener(m_animSet, this, STD_BIND_3(&AnimSetPanel::OnResourceEvent, this));
}

AnimSetPanel::~AnimSetPanel()
{
    // Dependencies
    GetResources()->UnregisterResourceListeners(m_animSet, this);

    SafeDelete(m_renderViewport);
}

void AnimSetPanel::OnUndoRedo()
{
    // TODO: keep animation and frame names, and try to restore selection.
    SelectAnimation(nullptr);
}

void AnimSetPanel::OnVisibilityChanged(bool visible)
{
    m_spriteAnimation->SetAnimationPause(!m_autoPlay || !visible);
}

void AnimSetPanel::UpdatePanelImpl(const DeltaTime& dt)
{
    // Toolbar.
    if (ImGui::SliderFloat("Zoom Factor", &m_zoomFactor, 0.f, 16.f))
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

    ImGui::SameLine();
    if (ImGui::Checkbox("Origin", &m_originFromAnimation))
    {
        m_spriteAnimation->SetOriginFromAnimation(m_originFromAnimation);

        if (!m_originFromAnimation)
        {
            m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
        }
    }

    ImGui::SameLine();
    if (ImGui::Checkbox("Move", &m_moveFromAnimation))
    {
        m_spriteAnimation->SetMoveFromAnimation(m_moveFromAnimation);

        if (!m_moveFromAnimation)
        {
            m_sprite->SetUnifiedPosition(UDim2::POSITION_CENTER);
        }
    }

    ImGui::SameLine();
    if (ImGui::Checkbox("Flip H", &m_flipH))
    {
        m_sprite->SetFlipH(m_flipH);
    }

    // Viewport.
    m_renderViewport->ImGuiBegin();
    m_renderViewport->ImGuiEnd();
}

void AnimSetPanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
    // AnimSet edition.
    ImageSet* mainImageSet = m_animSet->GetImageSet();
    std::string mainImageSetID = !mainImageSet ? "" : mainImageSet->GetID();
    if (ImGui::InputText("ImageSet", &mainImageSetID, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        ChangeMainImageSet(GetResources()->GetImageSet(mainImageSetID));
    }

    ImGui::Spacing();

    // Generators.
    ImGui::InputFloat("Default Duration", &m_defaultDuration);

    ImGui::BeginDisabled(m_currentAnimation == nullptr);
    if (ImGui::Button("Generate Animation Frames"))
    {
        OnGenerateAnimationFrames();
    }
    ImGui::EndDisabled();

    ImGui::Spacing();

    // Frame edition
    if (m_currentFrame)
    {
        ImGui::BeginDisabled(m_animSet->GetImageSet() == nullptr);

        SubImage* frameSubImage = !m_animSet->GetImageSet() ? nullptr : m_currentFrame->GetSubImage();
        std::string frameSubImageName = !frameSubImage ? "" : frameSubImage->GetName();
        if (ImGui::InputText("SubImage", &frameSubImageName, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_currentFrame->SetSubImage(m_animSet->GetImageSet()->GetSubImage(frameSubImageName));
            RaiseDirty();
        }

        ImGui::EndDisabled();

        Texture* frameTexture = m_currentFrame->GetTexture();
        std::string frameTextureId = !frameTexture ? "" : frameTexture->GetID();
        if (ImGui::InputText("Texture", &frameTextureId, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_currentFrame->SetTexture(GetResources()->GetTexture(frameTextureId));
            RaiseDirty();
        }

        Vector2f frameOrigin = m_currentFrame->GetOrigin();
        if (ImGui::InputFloat2("Origin", &frameOrigin))
        {
            m_currentFrame->SetOrigin(frameOrigin);
            RaiseDirty();
        }

        Vector2f frameMoveOffset = m_currentFrame->GetMoveOffset();
        if (ImGui::InputFloat2("Move Offset", &frameMoveOffset))
        {
            m_currentFrame->SetMoveOffset(frameMoveOffset);
            RaiseDirty();
        }
    }
    else
    {
        ImGui::BeginDisabled();

        std::string dummyStrA;
        std::string dummyStrB;
        Vector2f dummyVecA;
        Vector2f dummyVecB;
        ImGui::InputText("SubImage", &dummyStrA);
        ImGui::InputText("Texture", &dummyStrB);
        ImGui::InputFloat2("Origin", &dummyVecA);
        ImGui::InputFloat2("Move Offset", &dummyVecB);

        ImGui::EndDisabled();
    }

    ImGui::Spacing();

    // Animation edition buttons.
    {
        if (ImGui::Button("Add Animation"))
        {
            OnAddAnimation();
        }

        ImGui::SameLine();
        ImGui::BeginDisabled(m_currentAnimation == nullptr);
        if (ImGui::Button("Remove Animation"))
        {
            OnRemoveAnimation();
        }
        ImGui::EndDisabled();
    }

    // Animations list.
    ImGuiTableFlags animationTableflags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY /*| ImGuiTableFlags_NoPadInnerX */;
    if (ImGui::BeginTable("_ANIMATIONS_TABLE", 5, animationTableflags))
    {
        ImGuiTableColumnFlags animationColumnFlags = ImGuiTableColumnFlags_WidthFixed;
        ImGui::TableSetupColumn("name", animationColumnFlags, 120.f);
        ImGui::TableSetupColumn("play", animationColumnFlags, 30.f);
        ImGui::TableSetupColumn("duration", animationColumnFlags, 50.f);
        ImGui::TableSetupColumn("actions", animationColumnFlags, 40.f);
        ImGui::TableSetupColumn("remove", animationColumnFlags, 40.f);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        const std::vector<Animation*>& animations = m_animSet->GetAnimations();
        for (size_t animationIndex = 0; animationIndex < animations.size(); ++animationIndex)
        {
            ImGui::PushID((int)animationIndex);

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
                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
            }

            Animation* animation = animations[animationIndex];

            // Animation name.
            int columnIndex = 0;
            ImGui::TableSetColumnIndex(columnIndex++);

            ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
            if (ImGui::Selectable("##_ANIMATION_SELECTABLE", m_currentAnimation == animation, selectable_flags, ImVec2(0, row_min_height)))
            {
                if (m_currentAnimation != animation)
                {
                    SelectAnimation(animation);
                }
            }

            ImGui::SameLine();
            std::string animationName = animation->GetName();
            if (ImGui::InputText("##_ANIMATION_NAME", &animationName))
            {
                animation->SetName(animationName);
                RaiseDirty();
            }

            // Play cursor.
            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text(m_spriteAnimation->GetAnimation() == animation ? "<--" : "");

            // Empty column (used in frames section).
            ImGui::TableSetColumnIndex(columnIndex++);

            // Actions.
            ImGui::TableSetColumnIndex(columnIndex++);

            if (ImGui::Button("Add"))
            {
                AnimationFrame* referenceFrame = (animation->GetFrameCount() == 0) ? nullptr : animation->GetFrame(0);
                AnimationFrame* newFrame = animation->AddFrame(0);
                CopyFrame(newFrame, referenceFrame);

                RaiseDirty();
            }

            // Empty column (used in frames section).
            ImGui::TableSetColumnIndex(columnIndex++);

            // Frames.
            {
                ImGui::Indent();

                // Frames list.
                const std::vector<AnimationFrame*>& animationFrames = animation->GetFrames();
                for (size_t frameIndex = 0; frameIndex < animationFrames.size(); ++frameIndex)
                {
                    ImGui::PushID((int)frameIndex);

                    float frame_row_min_height = ImGui::GetFrameHeight();
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, frame_row_min_height);

                    AnimationFrame* frame = animationFrames[frameIndex];

                    // Frame name.
                    int frameColumnIndex = 0;
                    ImGui::TableSetColumnIndex(frameColumnIndex++);

                    std::string frameName = StringFormat("{0} [{1}]", animation->GetName(), frameIndex);

                    ImGuiSelectableFlags selectable_frame_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
                    if (ImGui::Selectable(frameName.c_str(), m_currentFrame == frame, selectable_frame_flags, ImVec2(0, frame_row_min_height)))
                    {
                        if (m_currentAnimation != animation)
                        {
                            SelectAnimation(animation);
                        }

                        m_currentFrame = frame;

                        if (!m_autoPlay)
                        {
                            m_spriteAnimation->SetCurrentFrame(frameIndex);
                        }
                    }

                    // Play cursor.
                    ImGui::TableSetColumnIndex(frameColumnIndex++);
                    ImGui::Text(m_spriteAnimation->GetAnimationFrame() == frame ? "<-" : "");

                    // Duration.
                    ImGui::TableSetColumnIndex(frameColumnIndex++);
                    float frameDuration = frame->GetDuration();
                    if (ImGui::InputFloat("##_FRAME_DURATION", &frameDuration))
                    {
                        frame->SetDuration(frameDuration);
                        RaiseDirty();
                    }

                    // Actions.
                    ImGui::TableSetColumnIndex(frameColumnIndex++);

                    if (ImGui::Button("Add"))
                    {
                        AnimationFrame* referenceFrame = frame;
                        AnimationFrame* newFrame = animation->AddFrame(frameIndex + 1);
                        CopyFrame(newFrame, referenceFrame);

                        RaiseDirty();
                    }

                    // Remove Frame.
                    ImGui::TableSetColumnIndex(frameColumnIndex++);

                    if (ImGui::Button("X"))
                    {
                        if (frame)
                        {
                            bool isCurrentFrame = m_currentFrame == frame;

                            size_t removedFrameIndex = animation->GetFrameIndex(frame);
                            animation->DeleteFrame(frame);

                            m_spriteAnimation->RestartAnimation();

                            if (isCurrentFrame)
                            {
                                m_currentFrame = animation->GetFrame(Min(animation->GetFrameCount() - 1, removedFrameIndex));
                            }

                            if (m_currentFrame)
                            {
                                m_spriteAnimation->SetCurrentFrame(m_currentFrame);
                            }

                            RaiseDirty();
                        }
                    }

                    ImGui::PopID();
                }

                ImGui::Unindent();
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

void AnimSetPanel::SelectAnimation(Animation* animation)
{
    m_currentAnimation = animation;
    m_currentFrame = nullptr;

    if (m_currentAnimation)
    {
        m_spriteAnimation->StartAnimation(m_currentAnimation);

        if (m_currentAnimation->GetFrameCount() > 0)
        {
            m_currentFrame = m_currentAnimation->GetFrame(0);
        }
    }
    else
    {
        m_spriteAnimation->StopAnimation();
    }
}

void AnimSetPanel::OnAddAnimation()
{
    Animation* newAnimation = m_animSet->AddAnimation(StringFormat("Animation_{0}", m_animSet->GetAnimations().size()));
    RaiseDirty();

    SelectAnimation(newAnimation);
}

void AnimSetPanel::OnRemoveAnimation()
{
    if (!m_currentAnimation)
        return;

    if (m_spriteAnimation->GetAnimation() == m_currentAnimation)
    {
        m_spriteAnimation->StopAnimation();
    }

    m_animSet->DeleteAnimation(m_currentAnimation);
    m_currentAnimation = nullptr;
    m_currentFrame = nullptr;

    RaiseDirty();
}

void AnimSetPanel::OnGenerateAnimationFrames()
{
    GetEditor()->OpenModalDialog(new GenerateAnimationFramesDialog(
        m_animSet->GetImageSet(),
        STD_BIND_1(&AnimSetPanel::GenerateAnimationFramesFromDirectory, this),
        STD_BIND_2(&AnimSetPanel::GenerateAnimationFramesFromImageSet, this)
    ));
}

void AnimSetPanel::GenerateAnimationFramesFromDirectory(const std::string& path)
{
    if (!m_currentAnimation)
        return;

    std::string targetPath = CombinePaths(GetEditor()->GetProjectSettings()->projectAssetsPath, path);

    if (!PathStartsWith(targetPath, GetEditor()->GetProjectSettings()->projectAssetsPath))
        return;

    std::vector<FileInfo> directoryFiles;
    GetFiles(targetPath, directoryFiles, false);

    std::sort(directoryFiles.begin(), directoryFiles.end());

    for (size_t i = 0; i < directoryFiles.size(); ++i)
    {
        const std::string& resourceID = GetResources()->GetResourceID(directoryFiles[i]);
        if (GetResources()->HasResource(resourceID))
        {
            Texture* texture = GetResources()->GetTexture(resourceID);
            if (texture)
            {
                AnimationFrame* newFrame = m_currentAnimation->AddFrame();
                newFrame->SetTexture(texture);
                newFrame->SetDuration(m_defaultDuration);
            }
        }
    }

    RaiseDirty();
}

void AnimSetPanel::GenerateAnimationFramesFromImageSet(size_t from, size_t to)
{
    if (!m_currentAnimation)
        return;

    ImageSet* imageSet = m_animSet->GetImageSet();
    if (!imageSet)
        return;

    const std::vector<SubImage*>& subImages = imageSet->GetSubImages();

    if (from > to || to >= subImages.size())
        return;

    for (size_t i = from; i <= to; ++i)
    {
        AnimationFrame* newFrame = m_currentAnimation->AddFrame();
        newFrame->SetSubImage(subImages[i]);
        newFrame->SetDuration(m_defaultDuration);
    }

    RaiseDirty();
}

void AnimSetPanel::ChangeMainImageSet(ImageSet* newImageSet)
{
    ImageSet* mainImageSet = m_animSet->GetImageSet();

    if (mainImageSet == newImageSet)
        return;

    const std::vector<Animation*>& animations = m_animSet->GetAnimations();
    for (size_t i = 0; i < animations.size(); ++i)
    {
        const std::vector<AnimationFrame*>& frames = animations[i]->GetFrames();
        for (size_t ii = 0; ii < frames.size(); ++ii)
        {
            // TODO: handle frames with explicit reference on an ImageSet (currently info is lost after loading).
            // TODO: deprecate multiple imagesets instead.
            if (!newImageSet)
            {
                frames[ii]->SetSubImage(nullptr);
            }
            else if (frames[ii]->GetSubImage())
            {
                frames[ii]->SetSubImage(newImageSet->GetSubImage(frames[ii]->GetSubImage()->GetName()));
            }
        }
    }

    m_animSet->SetImageSet(newImageSet);
    RaiseDirty();

    m_spriteAnimation->RestartAnimation();
}

void AnimSetPanel::CopyFrame(AnimationFrame* targetFrame, const AnimationFrame* referenceFrame)
{
    if (referenceFrame)
    {
        // TODO: copy ctor used in AddFrame ?
        targetFrame->SetDuration(referenceFrame->GetDuration());
        targetFrame->SetTexture(referenceFrame->GetTexture());
        targetFrame->SetSubImage(referenceFrame->GetSubImage());
        targetFrame->SetOrigin(referenceFrame->GetOrigin());
        targetFrame->SetMoveOffset(referenceFrame->GetMoveOffset());
    }
}

void AnimSetPanel::OnResourceEvent(const Resource* resource, EResourceEvent event, const Resource* dependency)
{
    if (event == EResourceEvent::DependencyRemoved
        || event == EResourceEvent::DependencyUpdated)
    {
        // TODO: If I keep subimage names in animation frames, I could directly handle retargeting in a OnDependencyUpdated method on the resource itself, and call a Restart here.
        ReloadCurrentState();
        SelectAnimation(nullptr);
    }
}

}   //namespace gugu
