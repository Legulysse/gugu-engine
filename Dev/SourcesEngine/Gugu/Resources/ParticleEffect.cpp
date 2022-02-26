////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ParticleEffect.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/External/PugiXmlWrap.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ParticleEffect::ParticleEffect()
{
}

ParticleEffect::~ParticleEffect()
{
}

EResourceType::Type ParticleEffect::GetResourceType() const
{
    return EResourceType::ParticleEffect;
}

ParticleSystemSettings* ParticleEffect::GetParticleSettings()
{
    return &m_particleSettings;
}

bool ParticleEffect::LoadFromFile()
{
    return true;
}

bool ParticleEffect::SaveToFile()
{
    pugi::xml_document docSave;

    pugi::xml_node nodeParticleEffect = docSave.append_child("ParticleEffect");

    return docSave.save_file(GetFileInfoRef().GetPathName().c_str());
}

}   // namespace gugu
