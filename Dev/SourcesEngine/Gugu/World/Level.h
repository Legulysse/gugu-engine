#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/DeltaTime.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Actor;
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class Level
{
	friend class Actor;

public:

    Level();
	virtual ~Level();

	virtual void Step(const DeltaTime& dt);
	virtual void Update(const DeltaTime& dt);
	
	//Render
    Element* GetRootNode() const;
    
    //Sub Levels
    Level* CreateSubLevel();
    Level* AddSubLevel(Level* _pLevel);
    void RemoveSubLevel(Level* _pLevel);
    void DeleteSubLevel(Level* _pLevel);
    void DeleteAllSubLevels();

    bool HasSubLevel(Level* _pLevel) const;
    Level* GetSubLevel(int _iIndex = 0) const;

    //Actors
    Actor* AddActor(Actor* _pActor);
    void RemoveActor(Actor* _pActor);
    void DeleteActor(Actor* _pActor);
    void DeleteAllActors();

    bool HasActor(Actor* _pActor) const;
    Actor* GetActor(int _iIndex = 0) const;
    int GetActorCount() const;

protected:

    void OnSubLevelReleased(Level* _pLevel);    //Called by a SubLevel just released, to ensure the Level is not referencing it.
    void OnActorReleased(Actor* _pActor);       //Called by an Actor just released, to ensure the Level is not referencing it.

protected:

    Level* m_parentLevel;
    std::vector<Level*> m_subLevels;
    std::vector<Actor*> m_actors;
    
    Element* m_rootNode;
};

}   // namespace gugu
