#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Actor.h"

#include <SFML/System/Vector2.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementSpriteAnimated;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Character : public gugu::Actor
{
public:

    Character();
    virtual ~Character();
	
    void	InitCharacter(bool bPlayer, float _fSpeed);

    void            Move        (sf::Vector2f _kDirection, const gugu::DeltaTime& dt);
    sf::Vector2f    GetPosition () const;

	void			Attack(const sf::Vector2f& _kCoords);

    bool TestCollision(class Projectile* _pProjectile);

	virtual void Step(const gugu::DeltaTime& dt) override;

public:

	float m_walkSpeed;		        // Distance walked per second

    float m_maxLife;                // Max Life

    float m_maxStamina;             // Max Stamina
    float m_staminaRecovery;        // Stamina recovery per second
    float m_staminaRecoveryDelay;   // Delay in seconds before stamina recovery

	float m_attackSpeed;	        // Nb attacks per second
    float m_attackStaminaCost;      // Stamina cost per attack

public:

	gugu::ElementSpriteAnimated* m_sprite;
    class ElementBar* m_lifeBar;

    float m_currentLife;
    float m_currentStamina;

    float m_attackCooldown;
    float m_staminaRecoveryCooldown;

    bool m_isDead;
};

}   //namespace demoproject
