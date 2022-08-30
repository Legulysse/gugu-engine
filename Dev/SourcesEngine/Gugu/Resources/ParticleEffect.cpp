////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ParticleEffect.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/External/PugiXmlWrap.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ParticleEffect::ParticleEffect()
{
}

ParticleEffect::~ParticleEffect()
{
    Unload();
}

EResourceType::Type ParticleEffect::GetResourceType() const
{
    return EResourceType::ParticleEffect;
}

void ParticleEffect::GetDependencies(std::set<Resource*>& dependencies) const
{
    if (m_particleSettings.imageSet)
    {
        dependencies.insert(m_particleSettings.imageSet);
    }
}

void ParticleEffect::OnDependencyRemoved(const Resource* removedDependency)
{
    if (m_particleSettings.imageSet == removedDependency)
    {
        m_particleSettings.imageSet = nullptr;
    }
}

ParticleSystemSettings* ParticleEffect::GetParticleSettings()
{
    return &m_particleSettings;
}

void ParticleEffect::Unload()
{
}

bool ParticleEffect::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node nodeParticleEffect = document.child("ParticleEffect");
    if (!nodeParticleEffect)
        return false;

    static const std::map<ParticleSystemSettings::EParticleShape, std::string> particleShapeEnumToString = {
        { ParticleSystemSettings::EParticleShape::Point, "Point" },
        { ParticleSystemSettings::EParticleShape::Quad, "Quad" },
    };

    static const std::map<ParticleSystemSettings::EEmitterShape, std::string> emitterShapeEnumToString = {
        { ParticleSystemSettings::EEmitterShape::Point, "Point" },
        { ParticleSystemSettings::EEmitterShape::Circle, "Circle" },
        { ParticleSystemSettings::EEmitterShape::Annulus, "Annulus" },
    };

    static const std::map<ParticleSystemSettings::EEmissionBehaviour, std::string> emissionBehaviourEnumToString = {
        { ParticleSystemSettings::EEmissionBehaviour::RandomDirection, "RandomDirection" },
        { ParticleSystemSettings::EEmissionBehaviour::AngleDirection, "AngleDirection" },
        { ParticleSystemSettings::EEmissionBehaviour::AwayFromCenter, "AwayFromCenter" },
    };

    static const std::map<std::string, ParticleSystemSettings::EParticleShape> particleShapeStringToEnum = {
        { "Point", ParticleSystemSettings::EParticleShape::Point },
        { "Quad",  ParticleSystemSettings::EParticleShape::Quad },
    };

    static const std::map<std::string, ParticleSystemSettings::EEmitterShape> emitterShapeStringToEnum = {
        { "Point", ParticleSystemSettings::EEmitterShape::Point },
        { "Circle",  ParticleSystemSettings::EEmitterShape::Circle },
        { "Annulus",  ParticleSystemSettings::EEmitterShape::Annulus },
    };

    static const std::map<std::string, ParticleSystemSettings::EEmissionBehaviour> emissionBehaviourStringToEnum = {
        { "RandomDirection", ParticleSystemSettings::EEmissionBehaviour::RandomDirection },
        { "AngleDirection",  ParticleSystemSettings::EEmissionBehaviour::AngleDirection },
        { "AwayFromCenter",  ParticleSystemSettings::EEmissionBehaviour::AwayFromCenter },
    };

    std::string particleShapeValue = particleShapeEnumToString.at(m_particleSettings.particleShape);
    std::string emitterShapeValue = emitterShapeEnumToString.at(m_particleSettings.emitterShape);
    std::string emissionBehaviourValue = emissionBehaviourEnumToString.at(m_particleSettings.emissionBehaviour);

    // Setup
    m_particleSettings.loop = nodeParticleEffect.child("Loop").attribute("value").as_bool(m_particleSettings.loop);
    m_particleSettings.duration = nodeParticleEffect.child("Duration").attribute("value").as_int(m_particleSettings.duration);
    m_particleSettings.maxParticleCount = nodeParticleEffect.child("MaxParticleCount").attribute("value").as_int(m_particleSettings.maxParticleCount);
    particleShapeValue = nodeParticleEffect.child("ParticleShape").attribute("value").as_string(particleShapeValue.c_str());
    m_particleSettings.useSortBuffer = nodeParticleEffect.child("SortBuffer").attribute("value").as_bool(m_particleSettings.useSortBuffer);
    m_particleSettings.localSpace = nodeParticleEffect.child("LocalSpace").attribute("value").as_bool(m_particleSettings.localSpace);

    // Emitter Shape
    emitterShapeValue = nodeParticleEffect.child("EmitterShape").attribute("value").as_string(emitterShapeValue.c_str());
    m_particleSettings.emitterRadius = nodeParticleEffect.child("EmitterRadius").attribute("value").as_float(m_particleSettings.emitterRadius);
    m_particleSettings.emitterInnerRadius = nodeParticleEffect.child("EmitterInnerRadius").attribute("value").as_float(m_particleSettings.emitterInnerRadius);

    // Particle Spawn
    pugi::xml_node nodeSpawnPerSecond = nodeParticleEffect.child("SpawnPerSecond");
    m_particleSettings.useRandomSpawnPerSecond = nodeSpawnPerSecond.attribute("random").as_bool(m_particleSettings.useRandomSpawnPerSecond);
    m_particleSettings.minSpawnPerSecond = nodeSpawnPerSecond.attribute("min").as_float(m_particleSettings.minSpawnPerSecond);
    m_particleSettings.maxSpawnPerSecond = nodeSpawnPerSecond.attribute("max").as_float(m_particleSettings.maxSpawnPerSecond);

    pugi::xml_node nodeParticlesPerSpawn = nodeParticleEffect.child("ParticlesPerSpawn");
    m_particleSettings.useRandomParticlesPerSpawn = nodeParticlesPerSpawn.attribute("random").as_bool(m_particleSettings.useRandomParticlesPerSpawn);
    m_particleSettings.minParticlesPerSpawn = nodeParticlesPerSpawn.attribute("min").as_int(m_particleSettings.minParticlesPerSpawn);
    m_particleSettings.maxParticlesPerSpawn = nodeParticlesPerSpawn.attribute("max").as_int(m_particleSettings.maxParticlesPerSpawn);

    // Particle behaviour
    emissionBehaviourValue = nodeParticleEffect.child("EmissionBehaviour").attribute("value").as_string(emissionBehaviourValue.c_str());

    pugi::xml_node nodeEmissionDirection = nodeParticleEffect.child("EmissionDirection");
    m_particleSettings.emissionDirection.x = nodeEmissionDirection.attribute("x").as_float(m_particleSettings.emissionDirection.x);
    m_particleSettings.emissionDirection.y = nodeEmissionDirection.attribute("y").as_float(m_particleSettings.emissionDirection.y);

    m_particleSettings.emissionAngle = nodeParticleEffect.child("EmissionAngle").attribute("value").as_float(m_particleSettings.emissionAngle);

    pugi::xml_node nodeLifetime = nodeParticleEffect.child("Lifetime");
    m_particleSettings.useRandomLifetime = nodeLifetime.attribute("random").as_bool(m_particleSettings.useRandomLifetime);
    m_particleSettings.minLifetime = nodeLifetime.attribute("min").as_int(m_particleSettings.minLifetime);
    m_particleSettings.maxLifetime = nodeLifetime.attribute("max").as_int(m_particleSettings.maxLifetime);

    pugi::xml_node nodeVelocity = nodeParticleEffect.child("Velocity");
    m_particleSettings.useRandomVelocity = nodeVelocity.attribute("random").as_bool(m_particleSettings.useRandomVelocity);
    m_particleSettings.minVelocity = nodeVelocity.attribute("min").as_float(m_particleSettings.minVelocity);
    m_particleSettings.maxVelocity = nodeVelocity.attribute("max").as_float(m_particleSettings.maxVelocity);

    // Render
    m_particleSettings.keepSizeRatio = nodeParticleEffect.child("KeepSizeRatio").attribute("value").as_bool(m_particleSettings.keepSizeRatio);

    pugi::xml_node nodeStartSize = nodeParticleEffect.child("StartSize");
    m_particleSettings.useRandomStartSize = nodeStartSize.attribute("random").as_bool(m_particleSettings.useRandomStartSize);
    m_particleSettings.minStartSize.x = nodeStartSize.attribute("min_x").as_float(m_particleSettings.minStartSize.x);
    m_particleSettings.minStartSize.y = nodeStartSize.attribute("min_y").as_float(m_particleSettings.minStartSize.y);
    m_particleSettings.maxStartSize.x = nodeStartSize.attribute("max_x").as_float(m_particleSettings.maxStartSize.x);
    m_particleSettings.maxStartSize.y = nodeStartSize.attribute("max_y").as_float(m_particleSettings.maxStartSize.y);

    m_particleSettings.updateSizeOverLifetime = nodeParticleEffect.child("UpdateSizeOverLifetime").attribute("value").as_bool(m_particleSettings.updateSizeOverLifetime);

    pugi::xml_node nodeEndSize = nodeParticleEffect.child("EndSize");
    m_particleSettings.useRandomEndSize = nodeEndSize.attribute("random").as_bool(m_particleSettings.useRandomEndSize);
    m_particleSettings.minEndSize.x = nodeEndSize.attribute("min_x").as_float(m_particleSettings.minEndSize.x);
    m_particleSettings.minEndSize.y = nodeEndSize.attribute("min_y").as_float(m_particleSettings.minEndSize.y);
    m_particleSettings.maxEndSize.x = nodeEndSize.attribute("max_x").as_float(m_particleSettings.maxEndSize.x);
    m_particleSettings.maxEndSize.y = nodeEndSize.attribute("max_y").as_float(m_particleSettings.maxEndSize.y);

    m_particleSettings.updateColorOverLifetime = nodeParticleEffect.child("UpdateColorOverLifetime").attribute("value").as_bool(m_particleSettings.updateColorOverLifetime);

    pugi::xml_node nodeStartColor = nodeParticleEffect.child("StartColor");
    m_particleSettings.startColor.r = (sf::Uint8)nodeStartColor.attribute("r").as_uint(m_particleSettings.startColor.r);
    m_particleSettings.startColor.g = (sf::Uint8)nodeStartColor.attribute("g").as_uint(m_particleSettings.startColor.g);
    m_particleSettings.startColor.b = (sf::Uint8)nodeStartColor.attribute("b").as_uint(m_particleSettings.startColor.b);
    m_particleSettings.startColor.a = (sf::Uint8)nodeStartColor.attribute("a").as_uint(m_particleSettings.startColor.a);

    pugi::xml_node nodeEndColor = nodeParticleEffect.child("EndColor");
    m_particleSettings.endColor.r = (sf::Uint8)nodeEndColor.attribute("r").as_uint(m_particleSettings.endColor.r);
    m_particleSettings.endColor.g = (sf::Uint8)nodeEndColor.attribute("g").as_uint(m_particleSettings.endColor.g);
    m_particleSettings.endColor.b = (sf::Uint8)nodeEndColor.attribute("b").as_uint(m_particleSettings.endColor.b);
    m_particleSettings.endColor.a = (sf::Uint8)nodeEndColor.attribute("a").as_uint(m_particleSettings.endColor.a);

    m_particleSettings.imageSet = GetResources()->GetImageSet(nodeParticleEffect.child("ImageSetID").attribute("value").as_string());

    // Finalize
    m_particleSettings.particleShape = particleShapeStringToEnum.at(particleShapeValue);
    m_particleSettings.emitterShape = emitterShapeStringToEnum.at(emitterShapeValue);
    m_particleSettings.emissionBehaviour = emissionBehaviourStringToEnum.at(emissionBehaviourValue);

    return true;
}

bool ParticleEffect::SaveToXml(pugi::xml_document& document) const
{
    pugi::xml_node nodeParticleEffect = document.append_child("ParticleEffect");

    static const std::map<ParticleSystemSettings::EParticleShape, std::string> particleShapeEnumToString = {
        { ParticleSystemSettings::EParticleShape::Point, "Point" },
        { ParticleSystemSettings::EParticleShape::Quad, "Quad" },
    };

    static const std::map<ParticleSystemSettings::EEmitterShape, std::string> emitterShapeEnumToString = {
        { ParticleSystemSettings::EEmitterShape::Point, "Point" },
        { ParticleSystemSettings::EEmitterShape::Circle, "Circle" },
        { ParticleSystemSettings::EEmitterShape::Annulus, "Annulus" },
    };

    static const std::map<ParticleSystemSettings::EEmissionBehaviour, std::string> emissionBehaviourEnumToString = {
        { ParticleSystemSettings::EEmissionBehaviour::RandomDirection, "RandomDirection" },
        { ParticleSystemSettings::EEmissionBehaviour::AngleDirection, "AngleDirection" },
        { ParticleSystemSettings::EEmissionBehaviour::AwayFromCenter, "AwayFromCenter" },
    };
    
    std::string particleShapeValue = particleShapeEnumToString.at(m_particleSettings.particleShape);
    std::string emitterShapeValue = emitterShapeEnumToString.at(m_particleSettings.emitterShape);
    std::string emissionBehaviourValue = emissionBehaviourEnumToString.at(m_particleSettings.emissionBehaviour);

    // Setup
    nodeParticleEffect.append_child("Loop").append_attribute("value").set_value(m_particleSettings.loop);
    nodeParticleEffect.append_child("Duration").append_attribute("value").set_value(m_particleSettings.duration);
    nodeParticleEffect.append_child("MaxParticleCount").append_attribute("value").set_value(m_particleSettings.maxParticleCount);
    nodeParticleEffect.append_child("ParticleShape").append_attribute("value").set_value(particleShapeValue.c_str());
    nodeParticleEffect.append_child("SortBuffer").append_attribute("value").set_value(m_particleSettings.useSortBuffer);
    nodeParticleEffect.append_child("LocalSpace").append_attribute("value").set_value(m_particleSettings.localSpace);

    // Emitter Shape
    nodeParticleEffect.append_child("EmitterShape").append_attribute("value").set_value(emitterShapeValue.c_str());
    nodeParticleEffect.append_child("EmitterRadius").append_attribute("value").set_value(m_particleSettings.emitterRadius);
    nodeParticleEffect.append_child("EmitterInnerRadius").append_attribute("value").set_value(m_particleSettings.emitterInnerRadius);

    // Particle Spawn
    pugi::xml_node nodeSpawnPerSecond = nodeParticleEffect.append_child("SpawnPerSecond");
    nodeSpawnPerSecond.append_attribute("random").set_value(m_particleSettings.useRandomSpawnPerSecond);
    nodeSpawnPerSecond.append_attribute("min").set_value(m_particleSettings.minSpawnPerSecond);
    nodeSpawnPerSecond.append_attribute("max").set_value(m_particleSettings.maxSpawnPerSecond);

    pugi::xml_node nodeParticlesPerSpawn = nodeParticleEffect.append_child("ParticlesPerSpawn");
    nodeParticlesPerSpawn.append_attribute("random").set_value(m_particleSettings.useRandomParticlesPerSpawn);
    nodeParticlesPerSpawn.append_attribute("min").set_value(m_particleSettings.minParticlesPerSpawn);
    nodeParticlesPerSpawn.append_attribute("max").set_value(m_particleSettings.maxParticlesPerSpawn);

    // Particle behaviour
    nodeParticleEffect.append_child("EmissionBehaviour").append_attribute("value").set_value(emissionBehaviourValue.c_str());

    pugi::xml_node nodeEmissionDirection = nodeParticleEffect.append_child("EmissionDirection");
    nodeEmissionDirection.append_attribute("x").set_value(m_particleSettings.emissionDirection.x);
    nodeEmissionDirection.append_attribute("y").set_value(m_particleSettings.emissionDirection.y);

    nodeParticleEffect.append_child("EmissionAngle").append_attribute("value").set_value(m_particleSettings.emissionAngle);

    pugi::xml_node nodeLifetime = nodeParticleEffect.append_child("Lifetime");
    nodeLifetime.append_attribute("random").set_value(m_particleSettings.useRandomLifetime);
    nodeLifetime.append_attribute("min").set_value(m_particleSettings.minLifetime);
    nodeLifetime.append_attribute("max").set_value(m_particleSettings.maxLifetime);

    pugi::xml_node nodeVelocity = nodeParticleEffect.append_child("Velocity");
    nodeVelocity.append_attribute("random").set_value(m_particleSettings.useRandomVelocity);
    nodeVelocity.append_attribute("min").set_value(m_particleSettings.minVelocity);
    nodeVelocity.append_attribute("max").set_value(m_particleSettings.maxVelocity);

    // Render
    nodeParticleEffect.append_child("KeepSizeRatio").append_attribute("value").set_value(m_particleSettings.keepSizeRatio);

    pugi::xml_node nodeStartSize = nodeParticleEffect.append_child("StartSize");
    nodeStartSize.append_attribute("random").set_value(m_particleSettings.useRandomStartSize);
    nodeStartSize.append_attribute("min_x").set_value(m_particleSettings.minStartSize.x);
    nodeStartSize.append_attribute("min_y").set_value(m_particleSettings.minStartSize.y);
    nodeStartSize.append_attribute("max_x").set_value(m_particleSettings.maxStartSize.x);
    nodeStartSize.append_attribute("max_y").set_value(m_particleSettings.maxStartSize.y);

    nodeParticleEffect.append_child("UpdateSizeOverLifetime").append_attribute("value").set_value(m_particleSettings.updateSizeOverLifetime);

    pugi::xml_node nodeEndSize = nodeParticleEffect.append_child("EndSize");
    nodeEndSize.append_attribute("random").set_value(m_particleSettings.useRandomEndSize);
    nodeEndSize.append_attribute("min_x").set_value(m_particleSettings.minEndSize.x);
    nodeEndSize.append_attribute("min_y").set_value(m_particleSettings.minEndSize.y);
    nodeEndSize.append_attribute("max_x").set_value(m_particleSettings.maxEndSize.x);
    nodeEndSize.append_attribute("max_y").set_value(m_particleSettings.maxEndSize.y);

    nodeParticleEffect.append_child("UpdateColorOverLifetime").append_attribute("value").set_value(m_particleSettings.updateColorOverLifetime);

    pugi::xml_node nodeStartColor = nodeParticleEffect.append_child("StartColor");
    nodeStartColor.append_attribute("r").set_value(m_particleSettings.startColor.r);
    nodeStartColor.append_attribute("g").set_value(m_particleSettings.startColor.g);
    nodeStartColor.append_attribute("b").set_value(m_particleSettings.startColor.b);
    nodeStartColor.append_attribute("a").set_value(m_particleSettings.startColor.a);

    pugi::xml_node nodeEndColor = nodeParticleEffect.append_child("EndColor");
    nodeEndColor.append_attribute("r").set_value(m_particleSettings.endColor.r);
    nodeEndColor.append_attribute("g").set_value(m_particleSettings.endColor.g);
    nodeEndColor.append_attribute("b").set_value(m_particleSettings.endColor.b);
    nodeEndColor.append_attribute("a").set_value(m_particleSettings.endColor.a);

    if (m_particleSettings.imageSet)
    {
        nodeParticleEffect.append_child("ImageSetID").append_attribute("value").set_value(m_particleSettings.imageSet->GetID().c_str());
    }
    else
    {
        nodeParticleEffect.append_child("ImageSetID").append_attribute("value");
    }

    return true;
}

}   // namespace gugu
