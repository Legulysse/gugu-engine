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
    Vector2f maxStartSize = Vector2f(5.f, 5.f);
    bool updateSizeOverLifetime = false;
    Vector2f minEndSize = Vector2f(0.f, 0.f);
    Vector2f maxEndSize = Vector2f(0.f, 0.f);
    sf::Color startColor = sf::Color::White;
    bool updateColorOverLifetime = false;
    sf::Color endColor = sf::Color::Black;
    std::string imageSetID = "";
};

}   //namespace gugu
