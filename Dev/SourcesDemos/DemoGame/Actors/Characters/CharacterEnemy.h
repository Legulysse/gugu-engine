#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Characters/Character.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class CharacterEnemy : public Character
{
public:

    CharacterEnemy();
    virtual ~CharacterEnemy();

    void InitEnemy(const class DS_Enemy* sheetEnemy, float _fSpeed, class Grid* grid, gugu::Element* parentNode);
};

}   //namespace demoproject
