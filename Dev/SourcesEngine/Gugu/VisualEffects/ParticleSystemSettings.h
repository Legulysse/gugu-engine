#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Color.hpp>

#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Texture;
    class ImageSet;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct ParticleSystemSettings
{
    enum class EParticleShape
    {
        Point,
        Quad,
    };

    enum class EEmitterShape
    {
        Point,
        //PointList,
        Circle,
        Annulus,
        //Square,
        //Segment,
        //SegmentList,
    };

    enum class EEmissionBehaviour
    {
        RandomDirection,
        AngleDirection,
        AwayFromCenter,
    };

    // Setup
    bool loop = true;
    int duration = 5000;
    bool useUnscaledTime = false;
    int maxParticleCount = 50;  // Hard-limit implemented at 100k particles, to avoid crashes.
    EParticleShape particleShape = EParticleShape::Quad;
    bool useSortBuffer = false;
    bool localSpace = false;

    // Emitter Shape
    EEmitterShape emitterShape = EEmitterShape::Point;
    float emitterRadius = 100.f;                    // Circle, Annulus
    float emitterInnerRadius = 50.f;                // Annulus.
    //Vector2f emitterSize = Vector2f(100.f, 100.f);  // Square.
    //std::vector<Vector2f> emissionPoints;           // PointList, Segment, SegmentList.

    // Particle Spawn
    bool useRandomSpawnPerSecond = false;
    float minSpawnPerSecond = 10.f;
    float maxSpawnPerSecond = 10.f;
    bool useRandomParticlesPerSpawn = false;
    int minParticlesPerSpawn = 1;
    int maxParticlesPerSpawn = 1;

    // Particle behaviour
    EEmissionBehaviour emissionBehaviour = EEmissionBehaviour::RandomDirection;
    //bool emissionDirectionInLocalSpace = false;
    Vector2f emissionDirection = Vector2f(0.f, -1.f);
    float emissionAngle = 90.f;
    bool useRandomLifetime = false;
    int minLifetime = 500;
    int maxLifetime = 500;
    bool useRandomVelocity = false;
    float minVelocity = 50.f;
    float maxVelocity = 50.f;

    // Render
    bool keepSizeRatio = true;
    bool useRandomStartSize = false;
    Vector2f minStartSize = Vector2f(5.f, 5.f);
    Vector2f maxStartSize = Vector2f(5.f, 5.f);
    bool updateSizeOverLifetime = false;
    bool useRandomEndSize = false;
    Vector2f minEndSize = Vector2f(0.f, 0.f);
    Vector2f maxEndSize = Vector2f(0.f, 0.f);
    bool updateColorOverLifetime = false;
    sf::Color startColor = sf::Color::White;
    sf::Color endColor = sf::Color::Black;
    Texture* texture = nullptr;
    ImageSet* imageSet = nullptr;
};

}   //namespace gugu
