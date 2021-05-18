////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/Level.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/World/Actor.h"
#include "Gugu/Element/Element.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Level::Level()
: m_parentLevel(nullptr)
{
    m_rootNode = new Element;
}

Level::~Level()
{
    //This will purge rendering bindings using this level
    GetEngine()->OnLevelReleased(this);

    if (m_parentLevel)
    {
        m_parentLevel->OnSubLevelReleased(this);
        m_parentLevel = nullptr;
    }

    DeleteAllActors();
    DeleteAllSubLevels();

    SafeDelete(m_rootNode);
}

void Level::Step(const DeltaTime& dt)
{
    StdVectorRemove<Level*>(m_subLevels, nullptr);
    StdVectorRemove<Actor*>(m_actors, nullptr);

    for (size_t i = 0; i < m_subLevels.size(); ++i)
    {
        if (m_subLevels[i])
            m_subLevels[i]->Step(dt);
    }

    for (size_t i = 0; i < m_actors.size(); ++i)
    {
        if (m_actors[i])
            m_actors[i]->Step(dt);
    }
    
    m_rootNode->Step(dt);
}

void Level::Update(const DeltaTime& dt)
{
    for (size_t i = 0; i < m_subLevels.size(); ++i)
    {
        if (m_subLevels[i])
            m_subLevels[i]->Update(dt);
    }

    for (size_t i = 0; i < m_actors.size(); ++i)
    {
        if (m_actors[i])
            m_actors[i]->Update(dt);
    }
    
    m_rootNode->Update(dt);
}

Element* Level::GetRootNode() const
{
    return m_rootNode;
}

Level* Level::CreateSubLevel()
{
    return AddSubLevel(new Level);
}

Level* Level::AddSubLevel(Level* _pLevel)
{
    if (!_pLevel)
        return nullptr;

    if (!StdVectorContains(m_subLevels, _pLevel))
    {
        m_subLevels.push_back(_pLevel);
        _pLevel->m_parentLevel = this;
        return _pLevel;
    }

    return nullptr;
}

void Level::RemoveSubLevel(Level* _pLevel)
{
    if (!_pLevel)
        return;

    auto iteLevel = StdVectorFind(m_subLevels, _pLevel);
    if (iteLevel != m_subLevels.end())
    {
        _pLevel->m_parentLevel = nullptr;
        *iteLevel = nullptr;
    }
}

void Level::DeleteSubLevel(Level* _pLevel)
{
    if (!_pLevel)
        return;

    RemoveSubLevel(_pLevel);
    SafeDelete(_pLevel);
}

void Level::DeleteAllSubLevels()
{
    for (size_t i = 0; i < m_subLevels.size(); ++i)
    {
        if (m_subLevels[i])
        {
            m_subLevels[i]->m_parentLevel = nullptr;   //Prevent the call to OnSubLevelReleased
            SafeDelete(m_subLevels[i]);
        }
    }
    m_subLevels.clear();
}

bool Level::HasSubLevel(Level* _pLevel) const
{
    if (!_pLevel)
        return false;

    return StdVectorContains(m_subLevels, _pLevel);
}

Level* Level::GetSubLevel(int _iIndex) const
{
    if (_iIndex < 0 || _iIndex >= (int)m_subLevels.size())
        return nullptr;
    return m_subLevels[_iIndex];
}

void Level::OnSubLevelReleased(Level* _pLevel)
{
    if (!_pLevel)
        return;

    auto iteLevel = StdVectorFind(m_subLevels, _pLevel);
    if (iteLevel != m_subLevels.end())
    {
        *iteLevel = nullptr;
    }
}

Actor* Level::AddActor(Actor* _pActor)
{
    if (!_pActor)
        return nullptr;

    if (!StdVectorContains(m_actors, _pActor))
    {
        m_actors.push_back(_pActor);
        _pActor->m_level = this;
        return _pActor;
    }

    return nullptr;
}

void Level::RemoveActor(Actor* _pActor)
{
    if (!_pActor)
        return;

    auto iteActor = StdVectorFind(m_actors, _pActor);
    if (iteActor != m_actors.end())
    {
        _pActor->m_level = nullptr;
        *iteActor = nullptr;
    }
}

void Level::DeleteActor(Actor* _pActor)
{
    if (!_pActor)
        return;

    RemoveActor(_pActor);
    SafeDelete(_pActor);
}

void Level::DeleteAllActors()
{
    for (size_t i = 0; i < m_actors.size(); ++i)
    {
        if (m_actors[i])
        {
            m_actors[i]->m_level = nullptr;    //Prevent the call to OnActorReleased
            SafeDelete(m_actors[i]);
        }
    }
    m_actors.clear();
}

bool Level::HasActor(Actor* _pActor) const
{
    if (!_pActor)
        return false;

    return StdVectorContains(m_actors, _pActor);
}

Actor* Level::GetActor(int _iIndex) const
{
    if (_iIndex < 0 || _iIndex >= (int)m_actors.size())
        return nullptr;
    return m_actors[_iIndex];
}

int Level::GetActorCount() const
{
    return (int)m_actors.size();
}

void Level::OnActorReleased(Actor* _pActor)
{
    if (!_pActor)
        return;

    auto iteActor = StdVectorFind(m_actors, _pActor);
    if (iteActor != m_actors.end())
    {
        *iteActor = nullptr;
    }
}

}   // namespace gugu
