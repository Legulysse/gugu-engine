////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ManagerResources.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Core/EngineConfig.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/Resources/Resource.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/Resources/AudioClip.h"
#include "Gugu/Resources/AudioMixerGroup.h"
#include "Gugu/Resources/SoundCue.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Resources/ParticleEffect.h"
#include "Gugu/Resources/Datasheet.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Resources/LocalizationTable.h"
#include "Gugu/Data/DataBindingUtility.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/Path.h"
#include "Gugu/System/Platform.h"
#include "Gugu/System/String.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerResources::ManagerResources()
{
    m_pathAssets = "";
    m_pathScreenshots = "";
    m_defaultFont = "";
    m_debugFont = "";
    m_useFullPath = false;
    m_defaultTextureSmooth = false;
    m_handleResourceDependencies = false;
}

ManagerResources::~ManagerResources()
{
}

bool ManagerResources::Init(const EngineConfig& config)
{
    m_pathAssets = config.pathAssets;
    m_pathScreenshots = config.pathScreenshots;
    m_useFullPath = config.useAssetsFullPaths;
    m_defaultFont = config.defaultFont;
    m_debugFont = config.debugFont;
    m_defaultTextureSmooth = config.defaultTextureSmooth;
    m_handleResourceDependencies = config.handleResourceDependencies;

    return ParseDirectory(m_pathAssets);
}

void ManagerResources::Release()
{
    m_resourceDependencies.clear();
    m_dataObjectFactories.clear();

    ClearStdMap(m_dataEnumInfos);
    ClearStdMap(m_customTextures);
    ClearStdMap(m_resources);
}

bool ManagerResources::ParseDirectory(std::string_view rootPath_utf8)
{
    //Parse Assets
    GetLogEngine()->Print(ELog::Info, ELogEngine::Resources, "Parsing Resources...");
    GetLogEngine()->Print(ELog::Info, ELogEngine::Resources, StringFormat("Root directory : {0}", rootPath_utf8));

    if (!DirectoryExists(rootPath_utf8))
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "The Root directory could not be found");
        return false;
    }

    std::vector<FileInfo> files;
    GetFiles(rootPath_utf8, files, true);

    size_t fileCount = 0;
    for (size_t i = 0; i < files.size(); ++i)
    {
        const FileInfo& fileInfos = files[i];
        std::string resourceId = (!m_useFullPath) ? std::string(fileInfos.GetFileName_utf8()) : std::string(fileInfos.GetFilePath_utf8().substr(rootPath_utf8.length()));

        if (RegisterResourceInfo(resourceId, fileInfos))
        {
            ++fileCount;
        }
    }

    GetLogEngine()->Print(ELog::Info, ELogEngine::Resources, StringFormat("Finished Parsing Resources (Found {0})", fileCount));
    return true;
}

const std::string& ManagerResources::GetPathAssets() const
{
    return m_pathAssets;
}

const std::string& ManagerResources::GetPathScreenshots() const
{
    return m_pathScreenshots;
}

bool ManagerResources::HasResource(const std::string& resourceId) const
{
    auto iteElement = m_resources.find(resourceId);
    return (iteElement != m_resources.end());
}

bool ManagerResources::IsResourceLoaded(const std::string& resourceId) const
{
    auto iteElement = m_resources.find(resourceId);
    if (iteElement != m_resources.end())
    {
        return (*iteElement).second->resource != nullptr;
    }

    return false;
}

bool ManagerResources::GetResourceFileInfo(const std::string& resourceId, FileInfo& fileInfo) const
{
    auto iteElement = m_resources.find(resourceId);
    if (iteElement != m_resources.end())
    {
        // This will return a copy.
        fileInfo = (*iteElement).second->fileInfo;
        return true;
    }

    return false;
}

const FileInfo& ManagerResources::GetResourceFileInfo(const std::string& resourceId) const
{
    auto iteElement = m_resources.find(resourceId);
    if (iteElement != m_resources.end())
    {
        return (*iteElement).second->fileInfo;
    }

    static const FileInfo defaultValue;
    return defaultValue;
}

void ManagerResources::PreloadAll()
{
    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        GetResource((*iteCurrent).second->resourceID);
        ++iteCurrent;
    }
}

void ManagerResources::SaveAll()
{
    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        if ((*iteCurrent).second->resource)
            (*iteCurrent).second->resource->SaveToFile();
        ++iteCurrent;
    }
}

EResourceType::Type ManagerResources::GetResourceType(const FileInfo& fileInfo) const
{
    if (fileInfo.HasExtension("png")
        || fileInfo.HasExtension("jpg")
        || fileInfo.HasExtension("jpeg")
        || fileInfo.HasExtension("tga")
        || fileInfo.HasExtension("bmp"))
    {
        return EResourceType::Texture;
    }
    else if (fileInfo.HasExtension("ttf"))
    {
        return EResourceType::Font;
    }
    else if (fileInfo.HasExtension("wav")
        || fileInfo.HasExtension("ogg")
        || fileInfo.HasExtension("flac")
        || fileInfo.HasExtension("mp3"))
    {
        return EResourceType::AudioClip;
    }
    else if (fileInfo.HasExtension("audiomixergroup.xml") || fileInfo.HasExtension("audiomixergroup"))
    {
        return EResourceType::AudioMixerGroup;
    }
    else if (fileInfo.HasExtension("soundcue.xml") || fileInfo.HasExtension("soundcue")
        || fileInfo.HasExtension("sound.xml") || fileInfo.HasExtension("sound"))
    {
        return EResourceType::SoundCue;
    }
    else if (fileInfo.HasExtension("imageset.xml") || fileInfo.HasExtension("imageset"))
    {
        return EResourceType::ImageSet;
    }
    else if (fileInfo.HasExtension("animset.xml") || fileInfo.HasExtension("animset"))
    {
        return EResourceType::AnimSet;
    }
    else if (fileInfo.HasExtension("particle.xml") || fileInfo.HasExtension("particle"))
    {
        return EResourceType::ParticleEffect;
    }
    //else if ()
    //{
    //    //TODO: check if the extension is a datasheet
    //}
    else if (fileInfo.HasExtension("widget.xml") || fileInfo.HasExtension("widget"))
    {
        return EResourceType::ElementWidget;
    }
    else if (fileInfo.HasExtension("localization.xml") || fileInfo.HasExtension("localization"))
    {
        return EResourceType::LocalizationTable;
    }
    else
    {
        return EResourceType::Unknown;
    }
}

Texture* ManagerResources::GetTexture(const std::string& resourceId)
{
    return dynamic_cast<Texture*>(GetResource(resourceId, EResourceType::Texture));
}

Font* ManagerResources::GetFont(const std::string& resourceId)
{
    return dynamic_cast<Font*>(GetResource(resourceId, EResourceType::Font));
}

AudioClip* ManagerResources::GetAudioClip(const std::string& resourceId)
{
    return dynamic_cast<AudioClip*>(GetResource(resourceId, EResourceType::AudioClip));
}

AudioMixerGroup* ManagerResources::GetAudioMixerGroup(const std::string& resourceId)
{
    return dynamic_cast<AudioMixerGroup*>(GetResource(resourceId, EResourceType::AudioMixerGroup));
}

SoundCue* ManagerResources::GetSoundCue(const std::string& resourceId)
{
    return dynamic_cast<SoundCue*>(GetResource(resourceId, EResourceType::SoundCue));
}

ImageSet* ManagerResources::GetImageSet(const std::string& resourceId)
{
    return dynamic_cast<ImageSet*>(GetResource(resourceId, EResourceType::ImageSet));
}

AnimSet* ManagerResources::GetAnimSet(const std::string& resourceId)
{
    return dynamic_cast<AnimSet*>(GetResource(resourceId, EResourceType::AnimSet));
}

ParticleEffect* ManagerResources::GetParticleEffect(const std::string& resourceId)
{
    return dynamic_cast<ParticleEffect*>(GetResource(resourceId, EResourceType::ParticleEffect));
}

Datasheet* ManagerResources::GetDatasheet(const std::string& resourceId)
{
    return dynamic_cast<Datasheet*>(GetResource(resourceId, EResourceType::Datasheet));
}

const DatasheetObject* ManagerResources::GetDatasheetRootObject(const std::string& resourceId)
{
    Datasheet* sheet = GetDatasheet(resourceId);
    return sheet == nullptr ? nullptr : sheet->GetRootObject();
}

ElementWidget* ManagerResources::GetElementWidget(const std::string& resourceId)
{
    return dynamic_cast<ElementWidget*>(GetResource(resourceId, EResourceType::ElementWidget));
}

LocalizationTable* ManagerResources::GetLocalizationTable(const std::string& resourceId)
{
    return dynamic_cast<LocalizationTable*>(GetResource(resourceId, EResourceType::LocalizationTable));
}

Resource* ManagerResources::GetResource(const std::string& resourceId, EResourceType::Type explicitType)
{
    if (resourceId.empty())
        return nullptr;

    //Check Resource already loaded
    auto iteResource = m_resources.find(resourceId);
    if (iteResource != m_resources.end())
    {
        if (iteResource->second->resource)
            return iteResource->second->resource;

        Resource* resource = LoadResource(iteResource->second, explicitType);
        return resource;
    }

    GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("GetResource failed, unknown resource : {0}", resourceId));
    return nullptr;
}

bool ManagerResources::LoadResource(const std::string& resourceId, EResourceType::Type explicitType)
{
    if (resourceId.empty())
        return false;

    auto iteElement = m_resources.find(resourceId);
    if (iteElement == m_resources.end())
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource failed, unknown resource : {0}", resourceId));
        return false;
    }

    return LoadResource(iteElement->second, explicitType) != nullptr;
}

Resource* ManagerResources::LoadResource(ResourceInfo* resourceInfo, EResourceType::Type explicitType)
{
    if (!resourceInfo)
        return nullptr;

    if (resourceInfo->resource)
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource ignored, resource already loaded : {0}", resourceInfo->resourceID));
        return resourceInfo->resource;
    }

    FileInfo fileInfo = resourceInfo->fileInfo;
    Resource* resource = nullptr;

    if (explicitType == EResourceType::Unknown)
    {
        explicitType = GetResourceType(fileInfo);
    }

    if (explicitType == EResourceType::Texture)
    {
        resource = new Texture;
    }
    else if (explicitType == EResourceType::Font)
    {
        resource = new Font;
    }
    else if (explicitType == EResourceType::AudioClip)
    {
        resource = new AudioClip;
    }
    else if (explicitType == EResourceType::AudioMixerGroup)
    {
        resource = new AudioMixerGroup;
    }
    else if (explicitType == EResourceType::SoundCue)
    {
        resource = new SoundCue;
    }
    else if (explicitType == EResourceType::ImageSet)
    {
        resource = new ImageSet;
    }
    else if (explicitType == EResourceType::AnimSet)
    {
        resource = new AnimSet;
    }
    else if (explicitType == EResourceType::ParticleEffect)
    {
        resource = new ParticleEffect;
    }
    else if (explicitType == EResourceType::Datasheet)
    {
        resource = new Datasheet;
    }
    else if (explicitType == EResourceType::ElementWidget)
    {
        resource = new ElementWidget;
    }
    else
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource failed, unknown resource extension : {0}", fileInfo.GetFilePath_utf8()));
    }

    if (resource)
    {
        resourceInfo->resource = resource;
        RegisterResourceDependencies(resource);

        resource->Init(resourceInfo);
        resource->LoadFromFile();

        UpdateResourceDependencies(resource);

        GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Resource loaded : {0}", resourceInfo->resourceID));
    }

    return resource;
}

bool ManagerResources::InjectResource(const std::string& resourceId, Resource* resource)
{
    if (resourceId.empty())
        return false;

    ResourceMapKey mapKey(resourceId);

    auto iteAsset = m_resources.find(mapKey);
    if (iteAsset != m_resources.end())
    {
        if (iteAsset->second->resource == nullptr)
        {
            iteAsset->second->resource = resource;
            RegisterResourceDependencies(resource);

            resource->Init(iteAsset->second);
            resource->LoadFromFile();

            UpdateResourceDependencies(resource);

            GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Injected Resource : {0}", resourceId));
            return true;
        }
        else
        {
            GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("InjectResource failed, the resource is already loaded : {0}", resourceId));
        }
    }
    else
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("InjectResource failed, the resource is unknown : {0}", resourceId));
    }

    return false;
}

const std::string& ManagerResources::GetResourceID(const Resource* resource) const
{
    if (resource)
    {
        auto iteCurrent = m_resources.begin();
        while (iteCurrent != m_resources.end())
        {
            if (iteCurrent->second->resource == resource)
                return iteCurrent->second->resourceID;
            ++iteCurrent;
        }
    }

    static const std::string defaultValue;
    return defaultValue;
}

const std::string& ManagerResources::GetResourceID(const FileInfo& fileInfo) const
{
    // TODO: I should be able to deduce an ID from a FileInfo.
    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        if (iteCurrent->second->fileInfo == fileInfo)
            return iteCurrent->second->resourceID;
        ++iteCurrent;
    }

    static const std::string defaultValue;
    return defaultValue;
}

bool ManagerResources::RegisterResourceInfo(const std::string& resourceId, const FileInfo& fileInfo)
{
    if (resourceId.empty())
        return false;

    ResourceMapKey mapKey(resourceId);

    auto iteAsset = m_resources.find(mapKey);
    if (iteAsset == m_resources.end())
    {
        ResourceInfo* resourceInfo = new ResourceInfo;
        resourceInfo->resourceID = resourceId;
        resourceInfo->fileInfo = fileInfo;
        resourceInfo->resource = nullptr;

        // TODO: investigate the use of hint iterator here, seems like a bad usage.
        m_resources.insert(iteAsset, std::make_pair(mapKey, resourceInfo));
        
        GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Registered Resource : ID = {0}, Path = {1}"
            , resourceId
            , fileInfo.GetFilePath_utf8()));
        
        return true;
    }
    
    GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("A Resource ID is already registered : ID = {0}, New Path = {1}, Registered Path = {2}"
        , resourceId
        , fileInfo.GetFilePath_utf8()
        , iteAsset->second->fileInfo.GetFilePath_utf8()));
        
    return false;
}

bool ManagerResources::AddResource(Resource* resource, const FileInfo& fileInfo)
{
    if (!resource)
        return false;

    std::string resourcePath = std::string(fileInfo.GetFilePath_utf8());
    std::string resourceName = std::string(fileInfo.GetFileName_utf8());

    if (resourceName.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("AddResource failed, name invalid : {0}", resourceName));
        return false;
    }

    std::string resourceId = resourceName;
    if (m_useFullPath)
        resourceId = resourcePath.substr(m_pathAssets.length());

    auto iteResource = m_resources.find(resourceId);
    if (iteResource != m_resources.end())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("AddResource failed, Resource already exists : {0}", resourceName));
        return false;
    }

    ResourceInfo* resourceInfo = new ResourceInfo;
    resourceInfo->resourceID = resourceId;
    resourceInfo->fileInfo = fileInfo;
    resourceInfo->resource = resource;

    m_resources.insert(iteResource, std::make_pair(resourceId, resourceInfo));
    RegisterResourceDependencies(resource);

    resource->Init(resourceInfo);

    UpdateResourceDependencies(resource);

    GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Added Resource : ID = {0}, Path = {1}"
        , resourceId
        , fileInfo.GetFilePath_utf8()));

    return true;
}

bool ManagerResources::MoveResource(Resource* resource, const FileInfo& fileInfo)
{
    if (!resource)
        return false;

    std::string resourcePath = std::string(fileInfo.GetFilePath_utf8());
    std::string resourceName = std::string(fileInfo.GetFileName_utf8());

    if (resourceName.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("RenameResource failed, new name invalid : {0}", resourceName));
        return false;
    }

    std::string resourceId = resourceName;
    if (m_useFullPath)
        resourceId = resourcePath.substr(m_pathAssets.length());

    auto iteNewResource = m_resources.find(resourceId);
    if (iteNewResource != m_resources.end())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("RenameResource failed, Resource already exists : {0}", resourceName));
        return false;
    }

    FileInfo previousFileInfo = resource->GetFileInfo();

    auto itePreviousResource = m_resources.find(resource->GetID());
    if (itePreviousResource == m_resources.end())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("RenameResource failed, Resource not found : {0}", previousFileInfo.GetFilePath_utf8()));
        return false;
    }

    ResourceInfo* resourceInfo = itePreviousResource->second;
    m_resources.erase(itePreviousResource);

    resourceInfo->resourceID = resourceId;
    resourceInfo->fileInfo = fileInfo;

    m_resources.insert(iteNewResource, std::make_pair(resourceId, resourceInfo));

    //Delete old file, save new file
    if (resource->SaveToFile())
    {
        RemoveFile(previousFileInfo.GetFilePath_utf8());
        return true;
    }

    GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Moved Resource : ID = {0}, Path = {1}"
        , resourceId
        , fileInfo.GetFilePath_utf8()));

    return false;
}

bool ManagerResources::RemoveResource(Resource* resource, bool unloadResource)
{
    if (!resource)
        return false;

    return RemoveResource(resource->GetID(), unloadResource);
}

bool ManagerResources::RemoveResource(const std::string& resourceId, bool unloadResource)
{
    auto iteResource = m_resources.find(resourceId);
    if (iteResource != m_resources.end())
    {
        Resource* resource = iteResource->second->resource;

        ResourceInfo* resourceInfo = iteResource->second;
        m_resources.erase(iteResource);

        if (resource)
        {
            NotifyResourceRemoved(resource);
            UnregisterResourceDependencies(resource);
        }

        if (!unloadResource)
        {
            resourceInfo->resource = nullptr;   // We want to keep the loaded resource in memory.
        }

        SafeDelete(resourceInfo);
    }

    return true;
}

bool ManagerResources::DeleteResource(Resource* resource)
{
    if (!resource)
        return false;

    return DeleteResource(resource->GetID());
}

bool ManagerResources::DeleteResource(const std::string& resourceId)
{
    auto iteResource = m_resources.find(resourceId);
    if (iteResource != m_resources.end())
    {
        FileInfo fileInfo = iteResource->second->fileInfo;

        if (RemoveResource(resourceId, true))
        {
            return RemoveFile(fileInfo.GetFilePath_utf8());
        }
    }

    return false;
}

void ManagerResources::RemoveResourcesFromPath(const std::string& path, bool unloadResources)
{
    if (path.empty())
        return;

    auto it = m_resources.begin();
    while (it != m_resources.end())
    {
        auto iteResource = it;
        ++it;

        if (PathStartsWith(iteResource->second->fileInfo.GetDirectoryPath_utf8(), path))
        {
            RemoveResource(iteResource->first, unloadResources);
        }
    }
}

Texture* ManagerResources::GetCustomTexture(const std::string& name)
{
    if (name.empty())
        return nullptr;

    auto iteElement = m_customTextures.find(name);
    if (iteElement != m_customTextures.end())
    {
        return (iteElement->second);
    }

    sf::Texture* newSFTexture = new sf::Texture;

    Texture* newTexture = new Texture;
    newTexture->SetSFTexture(newSFTexture);

    iteElement = m_customTextures.insert(iteElement, std::make_pair(name, newTexture));
    return newTexture;
}

bool ManagerResources::IsDefaultTextureSmooth() const
{
    return m_defaultTextureSmooth;
}

Font* ManagerResources::GetDefaultFont()
{
    return GetFont(m_defaultFont);
}

Font* ManagerResources::GetDebugFont()
{
    return GetFont(m_debugFont);
}

void ManagerResources::GetAllResourceInfos(std::vector<const ResourceInfo*>& resourceInfos) const
{
    resourceInfos.clear();
    resourceInfos.reserve(m_resources.size());

    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        resourceInfos.push_back(iteCurrent->second);
        ++iteCurrent;
    }
}

void ManagerResources::GetAllDatasheetsByType(std::string_view dataType, std::vector<Datasheet*>& datasheets)
{
    for (const auto& kvp : m_resources)
    {
        if (kvp.second->fileInfo.HasExtension(dataType))
        {
            Datasheet* datasheet = GetResources()->GetDatasheet(kvp.first);
            if (datasheet)
            {
                datasheets.push_back(datasheet);
            }
        }
    }
}

void ManagerResources::RegisterDataObjectFactory(const DelegateDataObjectFactory& delegateDataObjectFactory)
{
    m_dataObjectFactories.push_back(delegateDataObjectFactory);
}

DataObject* ManagerResources::InstanciateDataObject(std::string_view dataType)
{
    if (m_dataObjectFactories.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "No Data Object Factory registered");
    }
    else
    {
        for (size_t i = 0; i < m_dataObjectFactories.size(); ++i)
        {
            DataObject* dataObject = m_dataObjectFactories[i](dataType);
            if (dataObject)
            {
                return dataObject;
            }
        }
    }

    return nullptr;
}

void ManagerResources::RegisterDataEnumInfos(const std::string& name, const DataEnumInfos* enumInfos)
{
    auto iteElement = m_dataEnumInfos.find(name);
    if (iteElement != m_dataEnumInfos.end())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "Data Enum already registered");
        SafeDelete(enumInfos);
    }
    else
    {
        m_dataEnumInfos[name] = enumInfos;
    }
}

const DataEnumInfos* ManagerResources::GetDataEnumInfos(const std::string& name)
{
    auto iteElement = m_dataEnumInfos.find(name);
    if (iteElement != m_dataEnumInfos.end())
    {
        return (iteElement->second);
    }

    return nullptr;
}

void ManagerResources::RegisterResourceDependencies(Resource* resource)
{
    if (!m_handleResourceDependencies)
        return;

    auto it = m_resourceDependencies.find(resource);
    if (it == m_resourceDependencies.end())
    {
        it = m_resourceDependencies.insert(it, std::make_pair(resource, ResourceDependencies()));
    }
    else
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("RegisterResourceDependencies failed, Resource already registered : {0}", resource->GetID()));
    }
}

void ManagerResources::UpdateResourceDependencies(Resource* resource)
{
    if (!m_handleResourceDependencies)
        return;

    auto it = m_resourceDependencies.find(resource);
    if (it != m_resourceDependencies.end())
    {
        bool updatedDependencies = false;

        // Gather resource dependencies.
        if (it->second.dependencies.empty())
        {
            resource->GetDependencies(it->second.dependencies);

            updatedDependencies = !it->second.dependencies.empty();
        }
        else
        {
            std::set<Resource*> newDependencies;
            resource->GetDependencies(newDependencies);

            // Remove resource from its previous dependencies referencers.
            std::vector<Resource*> difference;
            std::set_difference(it->second.dependencies.begin(), it->second.dependencies.end(),
                newDependencies.begin(), newDependencies.end(), std::back_inserter(difference));

            for (const auto& dependency : difference)
            {
                auto itDependency = m_resourceDependencies.find(dependency);
                if (itDependency != m_resourceDependencies.end())
                {
                    itDependency->second.referencers.erase(resource);
                }
            }

            std::swap(it->second.dependencies, newDependencies);

            updatedDependencies = !difference.empty();
        }

        // Register resource to its dependencies referencers.
        for (const auto& dependency : it->second.dependencies)
        {
            auto itDependency = m_resourceDependencies.find(dependency);
            if (itDependency != m_resourceDependencies.end())
            {
                itDependency->second.referencers.insert(resource);
            }
        }

        if (updatedDependencies)
        {
            // If dependencies changed, we need to inform referencers in case they were depending on them too.
            for (const auto& referencer : it->second.referencers)
            {
                UpdateResourceDependencies(referencer);
            }
        }
    }
    else
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("UpdateResourceDependencies failed, Unregistered Resource : {0}", resource->GetID()));
    }
}

void ManagerResources::UnregisterResourceDependencies(Resource* resource)
{
    auto it = m_resourceDependencies.find(resource);
    if (it != m_resourceDependencies.end())
    {
        // Remove resource from its dependencies referencers.
        for (const auto& dependency : it->second.dependencies)
        {
            auto itDependency = m_resourceDependencies.find(dependency);
            if (itDependency != m_resourceDependencies.end())
            {
                itDependency->second.referencers.erase(resource);
            }
        }

        // Remove resource from its referencers dependencies.
        for (const auto& referencer : it->second.referencers)
        {
            auto itReferencer = m_resourceDependencies.find(referencer);
            if (itReferencer != m_resourceDependencies.end())
            {
                itReferencer->second.dependencies.erase(resource);
            }
        }

        // Remove resource from cache.
        m_resourceDependencies.erase(it);
    }
}

bool ManagerResources::RegisterResourceListener(const Resource* resource, const Handle& handle, const DelegateResourceEvent& delegateResourceEvent)
{
    if (!resource || !handle.IsValid())
        return false;

    auto it = m_resourceDependencies.find(resource);
    if (it != m_resourceDependencies.end())
    {
        ResourceListener resourceListener;
        resourceListener.handle = handle;
        resourceListener.delegateResourceEvent = delegateResourceEvent;

        it->second.listeners.push_back(resourceListener);
        return true;
    }
    else
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("RegisterResourceListener failed, Resource not loaded : {0}", resource->GetID()));
        return false;
    }
}

void ManagerResources::UnregisterResourceListeners(const Resource* resource, const Handle& handle)
{
    auto it = m_resourceDependencies.find(resource);
    if (it != m_resourceDependencies.end())
    {
        size_t i = 0;
        while (i < it->second.listeners.size())
        {
            if (it->second.listeners[i].handle == handle)
            {
                StdVectorRemoveAt(it->second.listeners, i);
            }
            else
            {
                ++i;
            }
        }
    }
}

void ManagerResources::UnregisterResourceListeners(const Handle& handle)
{
    // Remove all listeners originating from this handle.
    for (auto& kvp : m_resourceDependencies)
    {
        size_t i = 0;
        while (i < kvp.second.listeners.size())
        {
            if (kvp.second.listeners[i].handle == handle)
            {
                StdVectorRemoveAt(kvp.second.listeners, i);
            }
            else
            {
                ++i;
            }
        }
    }
}

void ManagerResources::NotifyResourceUpdated(const Resource* resource)
{
    auto it = m_resourceDependencies.find(resource);
    if (it != m_resourceDependencies.end())
    {
        // Notify referencers that a dependency has been updated.
        std::set<Resource*> referencers = it->second.referencers;
        for (const auto& referencer : referencers)
        {
            referencer->OnDependencyUpdated(resource);
        }

        for (const auto& referencer : referencers)
        {
            auto itReferencer = m_resourceDependencies.find(referencer);
            if (itReferencer != m_resourceDependencies.end())
            {
                std::vector<ResourceListener> listeners = itReferencer->second.listeners;
                for (const auto& resourceListener : listeners)
                {
                    resourceListener.delegateResourceEvent(referencer, EResourceEvent::DependencyUpdated, resource);
                }
            }
        }

        // Notify the resource itself being updated.
        std::vector<ResourceListener> listeners = it->second.listeners;
        for (const auto& resourceListener : listeners)
        {
            resourceListener.delegateResourceEvent(resource, EResourceEvent::ResourceUpdated, nullptr);
        }
    }
}

void ManagerResources::NotifyResourceRemoved(const Resource* resource)
{
    auto it = m_resourceDependencies.find(resource);
    if (it != m_resourceDependencies.end())
    {
        // Notify referencers that a dependency has been removed.
        std::set<Resource*> referencers = it->second.referencers;
        for (const auto& referencer : referencers)
        {
            referencer->OnDependencyRemoved(resource);
        }

        for (const auto& referencer : referencers)
        {
            auto itReferencer = m_resourceDependencies.find(referencer);
            if (itReferencer != m_resourceDependencies.end())
            {
                std::vector<ResourceListener> listeners = itReferencer->second.listeners;
                for (const auto& resourceListener : listeners)
                {
                    resourceListener.delegateResourceEvent(referencer, EResourceEvent::DependencyRemoved, resource);
                }
            }
        }

        // Notify the resource itself being removed.
        std::vector<ResourceListener> listeners = it->second.listeners;
        for (const auto& resourceListener : listeners)
        {
            resourceListener.delegateResourceEvent(resource, EResourceEvent::ResourceRemoved, nullptr);
        }
    }
}

const std::map<const Resource*, ManagerResources::ResourceDependencies>& ManagerResources::GetResourceDependencies() const
{
    return m_resourceDependencies;
}

ManagerResources* GetResources()
{
    return GetEngine()->GetManagerResources();
}

}   // namespace gugu
