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
    int verticesPerParticle = 6;
    bool localSpace = false;

    // Emitter
    float minSpawnPerSecond = 10.f;
    float maxSpawnPerSecond = 10.f;
    int minParticlesPerSpawn = 1;
    int maxParticlesPerSpawn = 1;

    // Particle behaviour
    int minLifetime = 500;
    int maxLifetime = 500;
    float minVelocity = 50.f;
    float maxVelocity = 100.f;

    // Render
    bool keepSizeRatio = true;
    Vector2f minStartSize = Vector2f(5.f, 5.f);
    Vector2f maxStartSize = Vector2f(10.f, 10.f);
    std::string imageSetID = "";
    //Vector2f minEndSize = Vector2f(5.f, 5.f); // TODO: lerp particle size over lifetime, we need to store a reference position to recompute the vertices.
    //Vector2f maxEndSize = Vector2f(10.f, 10.f);
    sf::Color startColor = sf::Color::Red;
    sf::Color endColor = sf::Color::Blue;
};

}   //namespace gugu
