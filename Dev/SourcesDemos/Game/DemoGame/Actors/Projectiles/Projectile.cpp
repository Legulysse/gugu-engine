////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Projectiles/Projectile.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Game/Game.h"

#include "DatasheetBinding.h"

#include "Gugu/Scene/Scene.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementParticles.h"
#include "Gugu/VisualEffects/ParticleSystemSettings.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Projectile::Projectile()
{
    m_sprite = nullptr;
    m_deathParticles = nullptr;
    m_isFireball = false;
    m_isBowAttack = false;
    m_speed = 650;
    m_lifetime = 2.f;
    m_isDead = false;
    m_pendingDestroy = false;
}

Projectile::~Projectile()
{
    SafeDelete(m_sprite);
    SafeDelete(m_deathParticles);
}

void Projectile::InitProjectile(const SkillContext& skillContext, const DS_EffectProjectile* effectSource, const Vector2f& _kFrom, const Vector2f& _kTo)
{
    m_skillContext = skillContext;
    m_effectSource = effectSource;

    m_characterSource = m_skillContext.caster;
    m_speed = effectSource->speed;

    m_lifetime = effectSource->lifetime;
    m_destination = _kTo;

    m_hitCount = 0;
    m_hasLifetime = m_lifetime > 0.f;
    m_hasDestination = m_effectSource->aim == EProjectileAim::Cursor;

    float targetDistance = 1.f;
    if (_kTo != _kFrom)
    {
        targetDistance = Length(_kTo - _kFrom);
        m_direction = Normalize(_kTo - _kFrom);
    }
    else
    {
        m_direction = Vector2f(1.f, 0.f);
    }

    Vector2f kOffset = Vector2f();
    if (!m_hasDestination)
    {
        // Add random amplitude
        float spreadAmplitude = InverseLerp(50.f, 400.f, Clamp(targetDistance, 50.f, 400.f));
        spreadAmplitude = 1.f - spreadAmplitude;
        spreadAmplitude = Lerp(0.05f, 1.25f, spreadAmplitude);
        m_direction = Rotate(m_direction, GetRandomf(-spreadAmplitude, spreadAmplitude));

        // Add random start offset
        kOffset = m_direction * GetRandomf(20.f, 25.f);

        // Add random speed
        m_speed += GetRandomf(-50, 50);
    }

    float fAngleDegrees = ToDegreesf(atan2f(m_direction.y, m_direction.x));     //TODO: Atan2 dans Math.h + AngleVector en version degrees + radians + rename ToDegrees

    if (m_skillContext.skill->name == "Bow Attack")
    {
        m_isBowAttack = true;
    }
    else if (m_skillContext.skill->name == "Fireball")
    {
        m_isFireball = true;
    }

    m_sprite = m_scene->GetRootNode()->AddChild<ElementSprite>();
    //m_sprite->SetTexture("Arrow.png");
    m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    m_sprite->SetRotation(fAngleDegrees + 90.f);
    m_sprite->SetPosition(_kFrom + kOffset);

    if (m_isFireball)
    {
        // Fireball trail
        ParticleSystemSettings particleSettings;
        particleSettings.maxParticleCount = 150;
        particleSettings.minSpawnPerSecond = 50;
        particleSettings.minParticlesPerSpawn = 5;
        particleSettings.useRandomStartSize = true;
        particleSettings.minStartSize = Vector2f(8.f, 8.f);
        particleSettings.maxStartSize = Vector2f(16.f, 16.f);
        particleSettings.updateSizeOverLifetime = true;
        particleSettings.minEndSize = Vector2f(6.f, 6.f);
        particleSettings.updateColorOverLifetime = true;
        particleSettings.startColor = sf::Color::Yellow;
        particleSettings.endColor = sf::Color(255, 0, 0, 100);

        ElementParticles* particles = m_sprite->AddChild<ElementParticles>();
        particles->CreateParticleSystem(particleSettings, true);
    }
    else if (m_isBowAttack)
    {
        m_sprite->SetTexture("Arrow.png");

        // Arrow trail
        ParticleSystemSettings particleSettings;
        particleSettings.maxParticleCount = 150;
        particleSettings.minSpawnPerSecond = 50;
        particleSettings.minParticlesPerSpawn = 5;
        particleSettings.minLifetime = 250;
        particleSettings.useRandomStartSize = true;
        particleSettings.minStartSize = Vector2f(1.f, 1.f);
        particleSettings.maxStartSize = Vector2f(2.f, 2.f);
        particleSettings.updateColorOverLifetime = true;
        particleSettings.startColor = sf::Color::Red;
        particleSettings.endColor = sf::Color(0, 0, 0, 0);

        ElementParticles* particles = m_sprite->AddChild<ElementParticles>();
        particles->CreateParticleSystem(particleSettings, true);
    }

    GetGame()->m_projectiles.push_back(this);
}

void Projectile::OnHit(Character* character)
{
    if (m_characterHits.find(character) == m_characterHits.end())
    {
        m_characterHits.insert(character);
        ++m_hitCount;

        SkillUtility::ApplySkillEffectList(m_skillContext, m_effectSource->effectsOnHit, character, m_sprite->GetPosition());

        if (m_effectSource->maximumHits >= 0)
        {
            m_isDead = m_hitCount >= m_effectSource->maximumHits;
        }
    }
}

void Projectile::Step(const DeltaTime& dt)
{
    SceneActor::Step(dt);

    if (m_isDead)
    {
        if (m_deathParticles)
        {
            m_lifetime -= dt.s();
            if (m_lifetime < 0.f)
            {
                m_pendingDestroy = true;
            }
        }
        else
        {
            m_pendingDestroy = true;

            if (m_isFireball)
            {
                m_pendingDestroy = false;
                m_lifetime = 3.f;

                // Fireball explosion
                ParticleSystemSettings particleSettings;
                particleSettings.loop = false;
                particleSettings.duration = 200;
                particleSettings.maxParticleCount = 16000;
                particleSettings.minSpawnPerSecond = 100;
                particleSettings.minParticlesPerSpawn = 2000;
                particleSettings.minLifetime = 400;
                particleSettings.useRandomVelocity = true;
                particleSettings.minVelocity = 100.f;
                particleSettings.maxVelocity = 500.f;
                particleSettings.useRandomStartSize = true;
                particleSettings.minStartSize = Vector2f(6.f, 6.f);
                particleSettings.maxStartSize = Vector2f(8.f, 8.f);
                particleSettings.updateColorOverLifetime = true;
                particleSettings.startColor = sf::Color::Yellow;
                particleSettings.endColor = sf::Color(255, 0, 0, 50);

                ElementParticles* particles = m_scene->GetRootNode()->AddChild<ElementParticles>();
                particles->SetPosition(m_sprite->GetPosition());
                particles->CreateParticleSystem(particleSettings, true);
                m_deathParticles = particles;

                //SafeDelete(m_sprite);
            }
            else if (m_isBowAttack)
            {
                m_pendingDestroy = false;
                m_lifetime = 3.f;

                // Bomb Arrow explosion
                ParticleSystemSettings particleSettings;
                particleSettings.loop = false;
                particleSettings.duration = 200;
                particleSettings.maxParticleCount = 500;
                particleSettings.minSpawnPerSecond = 50;
                particleSettings.minParticlesPerSpawn = 100;
                particleSettings.useRandomLifetime = true;
                particleSettings.minLifetime = 500;
                particleSettings.maxLifetime = 700;
                particleSettings.useRandomVelocity = true;
                particleSettings.minVelocity = 75.f;
                particleSettings.maxVelocity = 100.f;
                particleSettings.useRandomStartSize = true;
                particleSettings.minStartSize = Vector2f(3.f, 3.f);
                particleSettings.maxStartSize = Vector2f(8.f, 8.f);
                particleSettings.updateColorOverLifetime = true;
                particleSettings.startColor = sf::Color::Yellow;
                particleSettings.endColor = sf::Color(255, 0, 0, 50);

                ElementParticles* particles = m_scene->GetRootNode()->AddChild<ElementParticles>();
                particles->SetPosition(m_sprite->GetPosition());
                particles->CreateParticleSystem(particleSettings, true);
                m_deathParticles = particles;

                SafeDelete(m_sprite);
            }
        }
    }
    else
    {
        bool reachedDestination = false;
        Vector2f moveStep = m_direction * dt.s() * m_speed;

        if (m_hasDestination)
        {
            if (LengthSquare(m_sprite->GetPosition() - m_destination) <= LengthSquare(moveStep) + Math::Epsilon3)
            {
                reachedDestination = true;
            }
        }

        if (m_hasLifetime)
        {
            m_lifetime -= dt.s();
            if (m_lifetime < 0.f)
            {
                reachedDestination = true;
            }
        }

        if (reachedDestination && m_hasDestination)
        {
            m_sprite->SetPosition(m_destination);
        }
        else
        {
            m_sprite->Move(moveStep);
        }

        if (reachedDestination)
        {
            SkillUtility::ApplySkillEffectList(m_skillContext, m_effectSource->effectsOnDestination, nullptr, m_sprite->GetPosition());

            m_isDead = true;
        }
    }
}

}   //namespace demoproject
