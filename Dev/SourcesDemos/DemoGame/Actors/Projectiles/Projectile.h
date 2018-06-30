#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Actor.h"

#include <SFML/System/Vector2.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class Character;
}

namespace gugu
{
	class Element;
	class ElementSprite;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Projectile : public gugu::Actor
{
public:

	Projectile();
	virtual ~Projectile();

	void InitProjectile(Character* _pSource, const sf::Vector2f& _kFrom, const sf::Vector2f& _kTo);

	virtual void Step(const gugu::DeltaTime& dt) override;

public:

	Character* m_characterSource;
	gugu::ElementSprite* m_sprite;
	sf::Vector2f m_direction;
	float m_speed;
	float m_lifetime;
};

}   //namespace demoproject
