#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/DeltaTime.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Level;
}

namespace sf
{
	class RenderWindow;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class World
{
public:

	World();
	virtual ~World();

    void ResetWorld();

    Level* GetMainLevel() const;
    Level* GetPersistentLevel() const;

	void Step(const DeltaTime& dt);
	void Update(const DeltaTime& dt);

protected:

    Level* m_mainLevel;
    Level* m_persistentLevel;
};

World* GetWorld();

}   // namespace gugu
