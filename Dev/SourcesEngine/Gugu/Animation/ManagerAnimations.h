#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineConfig;
    class DeltaTime;
    class SpriteAnimation;
    class ElementSprite;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ManagerAnimations
{
public:

    ManagerAnimations();
    ~ManagerAnimations();

    void Init(const EngineConfig& config);
    void Release();

    void Step(const DeltaTime& dt);
    void Update(const DeltaTime& dt);

    SpriteAnimation* AddAnimation(ElementSprite* sprite);
    void RemoveAnimation(SpriteAnimation* animation);
    void DeleteAnimation(SpriteAnimation* animation);
    void DeleteAnimation(ElementSprite* sprite);
    void DeleteAllAnimations();

protected:

    std::vector<SpriteAnimation*> m_spriteAnimations;
};

ManagerAnimations* GetAnimations();

}   // namespace gugu
