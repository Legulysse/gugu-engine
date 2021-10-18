#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Color.hpp>

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct ParticleSystemSettings
{
    // Setup
    bool loop = true;
    int duration = 5000;
    int maxParticleCount = 50;
    int verticesPerParticle = 6;    // TODO: use an enum for particle type (point, quad).
    bool localSpace = false;

    // Emitter
    bool useRandomSpawnPerSecond = false;
    float minSpawnPerSecond = 10.f;
    float maxSpawnPerSecond = 10.f;
    bool useRandomParticlesPerSpawn = false;
    int minParticlesPerSpawn = 1;
    int maxParticlesPerSpawn = 1;

    // Particle behaviour
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
    std::string imageSetID = "";
};

}   //namespace gugu
