#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Callback.h"
#include "Gugu/System/Types.h"
//#include "Gugu/System/Hash.h"
#include "Gugu/System/FileInfo.h"
#include "Gugu/System/Handle.h"
#include "Gugu/Resources/EnumsResources.h"

#include <map>
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ResourceInfo;
    class Resource;
    class Texture;
    class Font;
    class AudioClip;
    class AudioMixerGroup;
    class SoundCue;
    class ImageSet;
    class AnimSet;
    class ParticleEffect;
    class Datasheet;
    class DataObject;
    class DatasheetObject;
    class ElementWidget;
    class LocalizationTable;
    struct DataEnumInfos;
    struct EngineConfig;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ManagerResources
{
public:

    using DelegateDataObjectFactory = std::function<DataObject* (std::string_view)>;
    using DelegateResourceEvent = std::function<void(const Resource* resource, EResourceEvent event, const Resource* dependency)>;    // TODO: Is dependency reference necessary ?

    struct ResourceListener
    {
        Handle handle;
        DelegateResourceEvent delegateResourceEvent = nullptr;
    };

    struct ResourceDependencies
    {
        std::set<Resource*> dependencies;
        std::set<Resource*> referencers;
        std::vector<ResourceListener> listeners;
    };

public:

    ManagerResources();
    ~ManagerResources();

    bool Init(const EngineConfig& config);
    void Release();

    const std::string& GetPathAssets() const;
    const std::string& GetPathScreenshots() const;
    
    bool HasResource(const std::string& resourceId) const;
    bool IsResourceLoaded(const std::string& resourceId) const;

    bool GetResourceFileInfo(const std::string& resourceId, FileInfo& fileInfo) const;
    const FileInfo& GetResourceFileInfo(const std::string& resourceId) const;

    bool ParseDirectory(std::string_view rootPath_utf8);
    void PreloadAll();
    void SaveAll();

    EResourceType::Type GetResourceType(const FileInfo& fileInfo) const;

    Texture* GetTexture(const std::string& resourceId);
    Font* GetFont(const std::string& resourceId);
    AudioClip* GetAudioClip(const std::string& resourceId);
    AudioMixerGroup* GetAudioMixerGroup(const std::string& resourceId);
    SoundCue* GetSoundCue(const std::string& resourceId);
    ImageSet* GetImageSet(const std::string& resourceId);
    AnimSet* GetAnimSet(const std::string& resourceId);
    ParticleEffect* GetParticleEffect(const std::string& resourceId);
    Datasheet* GetDatasheet(const std::string& resourceId);
    ElementWidget* GetElementWidget(const std::string& resourceId);
    LocalizationTable* GetLocalizationTable(const std::string& resourceId);

    template<typename T>
    const T* GetDatasheetObject(const std::string& resourceId)
    {
        return dynamic_cast<const T*>(GetDatasheetRootObject(resourceId));
    }

    Resource* GetResource(const std::string& resourceId, EResourceType::Type explicitType = EResourceType::Unknown);
    bool LoadResource(const std::string& resourceId, EResourceType::Type explicitType = EResourceType::Unknown);
    bool InjectResource(const std::string& resourceId, Resource* resource);

    // TODO: Obsolete editor getters ?
    const std::string& GetResourceID(const Resource* resource) const;
    const std::string& GetResourceID(const FileInfo& fileInfo) const;

    bool RegisterResourceInfo(const std::string& resourceId, const FileInfo& fileInfo);
    bool AddResource(Resource* resource, const FileInfo& fileInfo);
    bool MoveResource(Resource* resource, const FileInfo& fileInfo);

    bool RemoveResource(Resource* resource, bool unloadResource);
    bool RemoveResource(const std::string& resourceId, bool unloadResource);     // Remove resource (unloadResource will delete it from memory).
    bool DeleteResource(Resource* resource);
    bool DeleteResource(const std::string& resourceId);   // Remove resource, delete it from memory, erase file on disk.

    //TODO: Editor hack, waiting for ResourceContext to split editor and project resources.
    void RemoveResourcesFromPath(const std::string& path, bool unloadResources);

    void GetAllResourceInfos(std::vector<const ResourceInfo*>& resourceInfos) const;
    void GetAllDatasheetsByType(std::string_view dataType, std::vector<Datasheet*>& datasheets);

    //TODO: Refactor with ResourceContext
    Texture* GetCustomTexture(const std::string& name);

    bool IsDefaultTextureSmooth() const;

    Font* GetDefaultFont();
    Font* GetDebugFont();
    
    void RegisterDataObjectFactory(const DelegateDataObjectFactory& delegateDataObjectFactory);
    DataObject* InstanciateDataObject(std::string_view dataType);

    void RegisterDataEnumInfos(const std::string& name, const DataEnumInfos* enumInfos);
    const DataEnumInfos* GetDataEnumInfos(const std::string& name);

    bool RegisterResourceListener(const Resource* resource, const Handle& handle, const DelegateResourceEvent& delegateResourceEvent);
    void UnregisterResourceListeners(const Resource* resource, const Handle& handle);
    void UnregisterResourceListeners(const Handle& handle);

    void UpdateResourceDependencies(Resource* resource);
    const std::map<const Resource*, ResourceDependencies>& GetResourceDependencies() const;

    void NotifyResourceUpdated(const Resource* resource);

private:

    Resource* LoadResource(ResourceInfo* resourceInfo, EResourceType::Type explicitType = EResourceType::Unknown);

    const DatasheetObject* GetDatasheetRootObject(const std::string& resourceId);

    void RegisterResourceDependencies(Resource* resource);
    void UnregisterResourceDependencies(Resource* resource);
    void NotifyResourceRemoved(const Resource* resource);

private:

    //typedef Hash ResourceMapKey;
    typedef std::string ResourceMapKey;

    std::string m_pathAssets;
    std::string m_pathScreenshots;
    std::string m_defaultFont;
    std::string m_debugFont;
    bool m_useFullPath;         //TODO: some kind of enum RessourceIDPolicy.
    bool m_defaultTextureSmooth;
    bool m_handleResourceDependencies;

    std::map<ResourceMapKey, ResourceInfo*> m_resources;
    std::map<ResourceMapKey, Texture*> m_customTextures;

    std::vector<DelegateDataObjectFactory> m_dataObjectFactories;
    std::map<ResourceMapKey, const DataEnumInfos*> m_dataEnumInfos;

    std::map<const Resource*, ResourceDependencies> m_resourceDependencies;
};

ManagerResources* GetResources();

}   // namespace gugu
