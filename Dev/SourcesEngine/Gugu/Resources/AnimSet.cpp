////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/AnimSet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
AnimationFrame::AnimationFrame(Animation* _pAnimation)
: m_animation(_pAnimation)
, m_texture(nullptr)
, m_subImage(nullptr)
, m_duration(0.f)
{
}

AnimationFrame::~AnimationFrame()
{
}

Animation* AnimationFrame::GetAnimation() const
{
    return m_animation;
}

void AnimationFrame::SetTexture(Texture* _pTexture)
{
    m_texture = _pTexture;
}

Texture* AnimationFrame::GetTexture() const
{
    return m_texture;
}

void AnimationFrame::SetSubImage(SubImage* _pSubImage)
{
    m_subImage = _pSubImage;
}

SubImage* AnimationFrame::GetSubImage() const
{
    return m_subImage;
}

void AnimationFrame::SetDuration(float _fDuration)
{
    m_duration = _fDuration;
}

float AnimationFrame::GetDuration() const
{
    return m_duration;
}

void AnimationFrame::RegisterEvents(const std::string& events)
{
    StdStringSplit(events, "|", m_events);
}

bool AnimationFrame::HasEvents() const
{
    return m_events.size() > 0;
}

const std::vector<std::string>& AnimationFrame::GetEvents() const
{
    return m_events;
}

bool AnimationFrame::FillEventString(std::string& events) const
{
    if (m_events.empty())
        return false;

    for (const std::string& event : m_events)
    {
        if (!events.empty())
        {
            events += '|';
        }

        events += event;
    }

    return true;
}

void AnimationFrame::SetOrigin(const Vector2f _kOrigin)
{
    m_origin = _kOrigin;
}

const Vector2f& AnimationFrame::GetOrigin() const
{
    return m_origin;
}

void AnimationFrame::SetMoveOffset(const Vector2f _kMoveOffset)
{
    m_moveOffset = _kMoveOffset;
}

const Vector2f& AnimationFrame::GetMoveOffset() const
{
    return m_moveOffset;
}

Animation::Animation(AnimSet* _pAnimSet, const std::string& _strName)
: m_animSet(_pAnimSet)
, m_name("")
{
    SetName(_strName);
}

Animation::~Animation()
{
    ClearStdVector(m_frames);
}

AnimSet* Animation::GetAnimSet() const
{
    return m_animSet;
}

void Animation::SetName(const std::string& name)
{
    m_name = name;
}

bool Animation::IsName(const std::string& name) const
{
    return m_name == name;
}

const std::string& Animation::GetName() const
{
    return m_name;
}

AnimationFrame* Animation::AddFrame()
{
    return AddFrame(m_frames.size());
}

AnimationFrame* Animation::AddFrame(size_t insertIndex)
{
    AnimationFrame* pFrame = new AnimationFrame(this);
    StdVectorInsertAt(m_frames, insertIndex, pFrame);
    return pFrame;
}

void Animation::DeleteFrame(AnimationFrame* _pFrame)
{
    if (_pFrame)
    {
        StdVectorRemove(m_frames, _pFrame);
        SafeDelete(_pFrame);
    }
}

AnimationFrame* Animation::GetFrame(size_t _uiIndex) const
{
    if (_uiIndex >= 0 && _uiIndex < m_frames.size())
        return m_frames[_uiIndex];
    return nullptr;
}

size_t Animation::GetFrameIndex(AnimationFrame* _pFrame) const
{
    if (_pFrame)
    {
        for (size_t i = 0; i < m_frames.size(); ++i)
        {
            if (m_frames[i] == _pFrame)
                return i;
        }
    }

    return 0;
}

const std::vector<AnimationFrame*>& Animation::GetFrames() const
{
    return m_frames;
}

size_t Animation::GetFrameCount() const
{
    return m_frames.size();
}

AnimSet::AnimSet()
{
    m_imageSet = nullptr;
}

AnimSet::~AnimSet()
{
    Unload();
}

Animation* AnimSet::AddAnimation(const std::string& _strName)
{
    Animation* pNewAnimation = new Animation(this, _strName);
    m_animations.push_back(pNewAnimation);
    return pNewAnimation;
}

void AnimSet::DeleteAnimation(Animation* _pAnimation)
{
    if (_pAnimation)
    {
        StdVectorRemove(m_animations, _pAnimation);
        SafeDelete(_pAnimation);
    }
}

Animation* AnimSet::GetAnimation(const std::string& _strName) const
{
    for (size_t i = 0; i < m_animations.size(); ++i)
    {
        if (m_animations[i]->IsName(_strName))
        {
            return m_animations[i];
        }
    }

    return nullptr;
}

Animation* AnimSet::GetAnimation(size_t index) const
{
    if (index >= 0 && index < m_animations.size())
    {
        return m_animations[index];
    }

    return nullptr;
}

const std::vector<Animation*>& AnimSet::GetAnimations() const
{
    return m_animations;
}

void AnimSet::GetAnimationNames(std::vector<std::string>& _vecAnimationNames) const
{
    for (size_t i = 0; i < m_animations.size(); ++i)
    {
        _vecAnimationNames.push_back(m_animations[i]->GetName());
    }
}

ImageSet* AnimSet::GetImageSet() const
{
    return m_imageSet;
}

void AnimSet::SetImageSet(ImageSet* _pImageSet)
{
    m_imageSet = _pImageSet;
}

EResourceType::Type AnimSet::GetResourceType() const
{
    return EResourceType::AnimSet;
}

void AnimSet::GetDependencies(std::set<Resource*>& dependencies) const
{
    if (m_imageSet)
    {
        dependencies.insert(m_imageSet);

        // TODO: Do I need an option to retrieve or not indirect dependencies ?
        m_imageSet->GetDependencies(dependencies);
    }

    for (size_t i = 0; i < m_animations.size(); ++i)
    {
        const std::vector<AnimationFrame*>& frames = m_animations[i]->GetFrames();
        for (size_t ii = 0; ii < frames.size(); ++ii)
        {
            Texture* texture = frames[ii]->GetTexture();
            if (texture)
            {
                dependencies.insert(texture);
            }
        }
    }
}

void AnimSet::OnDependencyRemoved(const Resource* removedDependency)
{
    if (m_imageSet == removedDependency)
    {
        for (size_t i = 0; i < m_animations.size(); ++i)
        {
            const std::vector<AnimationFrame*>& frames = m_animations[i]->GetFrames();
            for (size_t ii = 0; ii < frames.size(); ++ii)
            {
                // TODO: handle frames with explicit reference on an ImageSet (currently info is lost after loading).
                // TODO: deprecate multiple imagesets instead.
                frames[ii]->SetSubImage(nullptr);
            }
        }

        m_imageSet = nullptr;
    }
}

void AnimSet::Unload()
{
    ClearStdVector(m_animations);
    m_imageSet = nullptr;
}

bool AnimSet::LoadFromXml(const pugi::xml_document& document)
{
    Unload();
    
    pugi::xml_node oNodeAnimSet = document.child("AnimSet");
    if (!oNodeAnimSet)
        return false;

    pugi::xml_attribute oAttributeMainImageSet = oNodeAnimSet.attribute("imageSet");
    if (oAttributeMainImageSet)
        m_imageSet = GetResources()->GetImageSet(oAttributeMainImageSet.as_string());

    for (pugi::xml_node oNodeAnimation = oNodeAnimSet.child("Animation"); oNodeAnimation; oNodeAnimation = oNodeAnimation.next_sibling("Animation"))
    {
        pugi::xml_attribute oAttributeAnimName = oNodeAnimation.attribute("name");
        if (oAttributeAnimName)
        {
            std::string strNameAnim = oAttributeAnimName.as_string();
            if (GetAnimation(strNameAnim))
                continue;

            Animation* pNewAnimation = AddAnimation(strNameAnim);

            for (pugi::xml_node oNodeFrame = oNodeAnimation.child("Frame"); oNodeFrame; oNodeFrame = oNodeFrame.next_sibling("Frame"))
            {
                AnimationFrame* pNewFrame = pNewAnimation->AddFrame();

                pugi::xml_attribute oAttributeFrameTexture = oNodeFrame.attribute("texture");
                if (oAttributeFrameTexture)
                    pNewFrame->SetTexture(GetResources()->GetTexture(oAttributeFrameTexture.as_string()));

                pugi::xml_attribute oAttributeSubImage = oNodeFrame.attribute("subImage");
                if (oAttributeSubImage)
                {
                    std::string strFrameSubImage = oAttributeSubImage.as_string();
                    ImageSet* pFrameImageSet = m_imageSet;

                    // TODO: deprecate multiple imagesets.
                    pugi::xml_attribute oAttributeNameSet = oNodeFrame.attribute("nameSet");
                    if (oAttributeNameSet)
                        pFrameImageSet = GetResources()->GetImageSet(oAttributeNameSet.as_string());

                    if (pFrameImageSet)
                        pNewFrame->SetSubImage(pFrameImageSet->GetSubImage(strFrameSubImage));
                }

                pugi::xml_attribute oAttributeDuration = oNodeFrame.attribute("duration");
                if (oAttributeDuration)
                    pNewFrame->SetDuration(oAttributeDuration.as_float());

                //TODO: Make this a child node instead of an attribute ?
                pugi::xml_attribute oAttributeEvents = oNodeFrame.attribute("events");
                if (oAttributeEvents)
                    pNewFrame->RegisterEvents(oAttributeEvents.as_string());

                Vector2f kOrigin;
                if (XmlReadVector2(oNodeFrame.child("Origin"), kOrigin))
                    pNewFrame->SetOrigin(kOrigin);

                Vector2f kMove;
                if (XmlReadVector2(oNodeFrame.child("Move"), kMove))
                    pNewFrame->SetMoveOffset(kMove);
            }
        }
    }

    return true;
}

bool AnimSet::SaveToXml(pugi::xml_document& document) const
{
    pugi::xml_node nodeAnimSet = document.append_child("AnimSet");
    nodeAnimSet.append_attribute("serializationVersion") = 1;

    nodeAnimSet.append_attribute("imageSet") = (!m_imageSet) ? "" : m_imageSet->GetID().c_str();

    for (Animation* animation : m_animations)
    {
        pugi::xml_node nodeAnimation = nodeAnimSet.append_child("Animation");
        nodeAnimation.append_attribute("name") = animation->GetName().c_str();

        const std::vector<AnimationFrame*>& frames = animation->GetFrames();
        for (const AnimationFrame* frame : frames)
        {
            pugi::xml_node nodeFrame = nodeAnimation.append_child("Frame");
            if (frame->GetTexture())
            {
                nodeFrame.append_attribute("texture") = frame->GetTexture()->GetID().c_str();
            }
            else if (frame->GetSubImage())
            {
                // TODO: deprecate multiple imagesets.
                if (m_imageSet != frame->GetSubImage()->GetImageSet())
                    nodeFrame.append_attribute("nameSet") = frame->GetSubImage()->GetImageSet()->GetID().c_str();
                nodeFrame.append_attribute("subImage") = frame->GetSubImage()->GetName().c_str();
            }

            nodeFrame.append_attribute("duration") = frame->GetDuration();

            std::string events;
            if (frame->FillEventString(events))
            {
                nodeFrame.append_attribute("events") = events.c_str();
            }

            if (frame->GetOrigin() != Vector2::Zero_f)
            {
                XmlWriteVector2(nodeFrame.append_child("Origin"), frame->GetOrigin());
            }

            if (frame->GetMoveOffset() != Vector2::Zero_f)
            {
                XmlWriteVector2(nodeFrame.append_child("Move"), frame->GetMoveOffset());
            }
        }
    }

    return true;
}

}   // namespace gugu
