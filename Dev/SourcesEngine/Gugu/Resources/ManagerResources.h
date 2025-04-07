#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Callback.h"
#include "Gugu/System/Types.h"
//#include "Gugu/System/Hash.h"
#include "Gugu/System/FileInfo.h"
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
    class Sound;
    class Music;
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
    struct DataEnumInfos;
    struct EngineConfig;
}

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
    class Music;
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
        const void* handle = nullptr;
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
    
    bool HasResource(const std::string& _strName) const;
    bool IsResourceLoaded(const std::string& _strName) const;

    bool GetResourceFileInfo(const std::string& _strName, FileInfo& fileInfo) const;
    const FileInfo& GetResourceFileInfo(const std::string& _strName) const;

    bool ParseDirectory(std::string_view rootPath_utf8);
    void PreloadAll();
    void SaveAll();

    EResourceType::Type GetResourceType(const FileInfo& fileInfo) const;

    Texture*    GetTexture      (const std::string& _strName);
    Font*       GetFont         (const std::string& _strName);
    Sound*      GetSound        (const std::string& _strName);
    Music*      GetMusic        (const std::string& _strName);
    AudioClip* GetAudioClip(const std::string& _strName);
    AudioMixerGroup* GetAudioMixerGroup(const std::string& _strName);
    SoundCue*   GetSoundCue     (const std::string& _strName);
    ImageSet*   GetImageSet     (const std::string& _strName);
    AnimSet*    GetAnimSet      (const std::string& _strName);
    ParticleEffect* GetParticleEffect(const std::string& _strName);
    Datasheet*  GetDatasheet    (const std::string& _strName);
    ElementWidget* GetElementWidget(const std::string& _strName);

    template<typename T>
    const T* GetDatasheetObject(const std::string& _strName)
    {
        return dynamic_cast<const T*>(GetDatasheetRootObject(_strName));
    }

    Resource* GetResource(const std::string& _strName, EResourceType::Type _eExplicitType = EResourceType::Unknown);
    bool LoadResource(const std::string& _strName, EResourceType::Type _eExplicitType = EResourceType::Unknown);
    bool InjectResource(const std::string& _strName, Resource* _pResource);

    // TODO: Obsolete editor getters ?
    const std::string& GetResourceID(const Resource* _pResource) const;
    const std::string& GetResourceID(const FileInfo& _oFileInfo) const;

    bool        RegisterResourceInfo(const std::string& _strResourceID, const FileInfo& _kFileInfos);
    bool        AddResource     (Resource* _pNewResource, const FileInfo& _oFileInfo);
    bool        MoveResource    (Resource* _pResource, const FileInfo& _oFileInfo);

    bool        RemoveResource  (Resource* _pResource);
    bool        RemoveResource  (const std::string& resourceID);
    bool        DeleteResource  (Resource* _pResource);
    bool        DeleteResource  (const std::string& resourceID);

    //TODO: Editor hack, waiting for ResourceContext to split editor and project resources.
    void        RemoveResourcesFromPath (const std::string& _strPath);

    void GetAllResourceInfos(std::vector<const ResourceInfo*>& _vecInfos) const;
    void GetAllDatasheetsByType(std::string_view dataType, std::vector<Datasheet*>& datasheets);

    // TODO: Obsolete editor getters ?
    //void        GetLoadedResourceInfos  (std::vector<const ResourceInfo*>& _vecInfos, EResourceType::Type _eType = EResourceType::Unknown) const;
    //void        GetLoadedImageSetInfos  (std::vector<const ResourceInfo*>& _vecInfos) const;
    //void        GetLoadedAnimSetInfos   (std::vector<const ResourceInfo*>& _vecInfos) const;
    //void        GetLoadedTextureInfos   (std::vector<const ResourceInfo*>& _vecInfos) const;
    //void        GetResourceInfosFromPath(std::vector<const ResourceInfo*>& _vecInfos, const std::string& _strPath, EResourceType::Type _eType = EResourceType::Unknown) const;

    //TODO: Refactor with ResourceContext
    Texture*        GetCustomTexture(const std::string& _strName);

    bool            IsDefaultTextureSmooth() const;

    Font*           GetDefaultFont();
    Font*           GetDebugFont();
    
    void            RegisterDataObjectFactory  (const DelegateDataObjectFactory& delegateDataObjectFactory);
    DataObject*     InstanciateDataObject      (std::string_view dataType);

    void RegisterDataEnumInfos(const std::string& _strName, const DataEnumInfos* _pEnum);
    const DataEnumInfos* GetDataEnumInfos(const std::string& _strName);

    bool RegisterResourceListener(const Resource* resource, const void* handle, const DelegateResourceEvent& delegateResourceEvent);
    void UnregisterResourceListeners(const Resource* resource, const void* handle);
    void UnregisterResourceListeners(const void* handle);

    void UpdateResourceDependencies(Resource* resource);
    const std::map<const Resource*, ResourceDependencies>& GetResourceDependencies() const;

    void NotifyResourceUpdated(const Resource* resource);

private:

    Resource* LoadResource(ResourceInfo* _pResourceInfo, EResourceType::Type _eExplicitType = EResourceType::Unknown);

    const DatasheetObject* GetDatasheetRootObject(const std::string& _strName);

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
