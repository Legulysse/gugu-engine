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
#include "Gugu/External/ImGuiWrapper.h"

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
}

AnimSetPanel::~AnimSetPanel()
{
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
#if 1
    //const ImGuiStyle& style = ImGui::GetStyle();

    //bool changedSelectionThisFrame = false;

    //ImGuiTreeNodeFlags animationNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
    //ImGuiTreeNodeFlags frameNodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;


    // AnimSet edition.
    ImageSet* mainImageSet = m_animSet->GetImageSet();
    std::string mainImageSetID = !mainImageSet ? "" : mainImageSet->GetID();
    if (ImGui::InputText("ImageSet", &mainImageSetID, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        UpdateMainImageSet(GetResources()->GetImageSet(mainImageSetID));
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

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text(m_spriteAnimation->GetAnimation() == animation ? "<--" : "");

            // Empty column.
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

            // Remove.
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

                        AnimationFrame* frame = animationFrames[frameIndex];

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

                        ImGui::TableSetColumnIndex(frameColumnIndex++);
                        ImGui::Text(m_spriteAnimation->GetAnimationFrame() == frame ? "<-" : "");

                        //ImGui::TableSetColumnIndex(frameColumnIndex++);
                        //ImGui::Text("%f", animationFrames[frameIndex]->GetDuration());

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

                        // Remove.
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
        std::bind(&AnimSetPanel::GenerateAnimationFramesFromDirectory, this, std::placeholders::_1),
        std::bind(&AnimSetPanel::GenerateAnimationFramesFromImageSet, this, std::placeholders::_1, std::placeholders::_2)
    ));
}

void AnimSetPanel::GenerateAnimationFramesFromDirectory(const std::string& path)
{
    if (!m_currentAnimation)
        return;

    std::string targetPath = CombinePaths(GetEditor()->GetProjectSettings()->projectAssetsPath, path, false);

    if (!StdStringStartsWith(targetPath, GetEditor()->GetProjectSettings()->projectAssetsPath))
        return;

    std::vector<FileInfo> directoryFiles;
    GetFiles(targetPath, directoryFiles, false);

    std::sort(directoryFiles.begin(), directoryFiles.end());

    for (size_t i = 0; i < directoryFiles.size(); ++i)
    {
        std::string resourceID = GetResources()->GetResourceID(directoryFiles[i]);
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

void AnimSetPanel::UpdateMainImageSet(ImageSet* newImageSet)
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

}   //namespace gugu
