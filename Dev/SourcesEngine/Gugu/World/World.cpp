////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/World.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/World/Level.h"
#include "Gugu/Utility/System.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

World::World()
: m_mainLevel(nullptr)
, m_persistentLevel(nullptr)
{
}

World::~World()
{
    SafeDelete(m_persistentLevel);
    SafeDelete(m_mainLevel);
}

void World::ResetWorld()
{
    SafeDelete(m_persistentLevel);
    SafeDelete(m_mainLevel);

    m_mainLevel = new Level;
    m_persistentLevel = new Level;
}

Level* World::GetMainLevel() const
{
    return m_mainLevel;
}

Level* World::GetPersistentLevel() const
{
    return m_persistentLevel;
}

void World::Step(const DeltaTime& dt)
{
    if (m_mainLevel)
    {
        m_mainLevel->Step(dt);
    }
    if (m_persistentLevel)
    {
        m_persistentLevel->Step(dt);
    }
}

void World::Update(const DeltaTime& dt)
{
    if (m_mainLevel)
    {
        m_mainLevel->Update(dt);
    }
    if (m_persistentLevel)
    {
        m_persistentLevel->Update(dt);
    }
}

World* GetWorld()
{
    return GetEngine()->GetWorld();
}

}   // namespace gugu
