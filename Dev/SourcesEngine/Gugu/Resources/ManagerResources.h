#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Callback.h"
#include "Gugu/System/Types.h"
#include "Gugu/System/Hash.h"
#include "Gugu/System/FileInfo.h"
#include "Gugu/Resources/EnumsResources.h"

#include <map>
#include <vector>

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
    class SoundCue;
    class ImageSet;
    class AnimSet;
    class ParticleEffect;
    class Datasheet;
    class DatasheetObject;
    struct DatasheetEnum;
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

    using DelegateDatasheetObjectFactory = std::function<DatasheetObject* (const std::string&)>;
    using DelegateResourceUpdated = std::function<void(const Resource* resource, bool removed)>;

public:

    ManagerResources();
    ~ManagerResources();

    void Init(const EngineConfig& config);
    void Release();

    std::string GetPathAssets       () const;
    std::string GetPathScreenshots  () const;
    
    bool HasResource(const std::string& _strName) const;
    bool IsResourceLoaded(const std::string& _strName) const;

    bool GetResourceFileInfo(const std::string& _strName, FileInfo& fileInfo) const;
    bool GetResourcePathName(const std::string& _strName, std::string& pathName) const;
    FileInfo GetResourceFileInfo (const std::string& _strName) const;
    std::string GetResourcePathName (const std::string& _strName) const;

    void        ParseDirectory  (const std::string& _strPath);
    void        PreloadAll      ();
    void        SaveAll         ();

    EResourceType::Type GetResourceType(const FileInfo& fileInfo) const;

    Texture*    GetTexture      (const std::string& _strName);
    Font*       GetFont         (const std::string& _strName);
    Sound*      GetSound        (const std::string& _strName);
    Music*      GetMusic        (const std::string& _strName);
    SoundCue*   GetSoundCue     (const std::string& _strName);
    ImageSet*   GetImageSet     (const std::string& _strName);
    AnimSet*    GetAnimSet      (const std::string& _strName);
    ParticleEffect* GetParticleEffect(const std::string& _strName);
    Datasheet*  GetDatasheet    (const std::string& _strName);

    template<typename T>
    const T* GetDatasheetObject(const std::string& _strName)
    {
        return dynamic_cast<const T*>(GetDatasheet(_strName)->GetRootObject());
    }

    Resource* GetResource(const std::string& _strName, EResourceType::Type _eExplicitType = EResourceType::Unknown);
    bool LoadResource(const std::string& _strName, EResourceType::Type _eExplicitType = EResourceType::Unknown);
    bool InjectResource(const std::string& _strName, Resource* _pResource);

    // TODO: Obsolete editor getters ?
    std::string GetResourceID   (const Resource* _pResource) const;
    std::string GetResourceID   (const FileInfo& _oFileInfo) const;

    bool        RegisterResourceInfo(const std::string& _strResourceID, const FileInfo& _kFileInfos);
    bool        AddResource     (Resource* _pNewResource, const FileInfo& _oFileInfo);
    bool        MoveResource    (Resource* _pResource, const FileInfo& _oFileInfo);

    bool        RemoveResource  (Resource* _pResource);
    bool        RemoveResource  (const std::string& resourceID);
    bool        DeleteResource  (Resource* _pResource);
    bool        DeleteResource  (const std::string& resourceID);

    //TODO: Editor hack, waiting for ResourceContext to split editor and project resources.
    void        RemoveResourcesFromPath (const std::string& _strPath);

    void        GetAllResourceInfos(std::vector<const ResourceInfo*>& _vecInfos) const;

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
    
    void                RegisterDatasheetObjectFactory    (const DelegateDatasheetObjectFactory& delegateDatasheetObjectFactory);
    DatasheetObject*    InstanciateDatasheetObject        (const std::string& _strType);

    void RegisterDatasheetEnum(const std::string& _strName, const DatasheetEnum* _pEnum);
    const DatasheetEnum* GetDatasheetEnum(const std::string& _strName);

    void RegisterResourceListenerOnDependencies(const Resource* resource, const void* handle, const DelegateResourceUpdated& delegateResourceUpdated);
    void RegisterResourceListener(const Resource* resource, const void* handle, const DelegateResourceUpdated& delegateResourceUpdated);
    void UnregisterResourceListeners(const void* handle);
    void RemoveAllResourceListeners(const Resource* resource);
    void TriggerResourceUpdated(const Resource* resource, bool removed);

private:

    Resource* LoadResource(ResourceInfo* _pResourceInfo, EResourceType::Type _eExplicitType = EResourceType::Unknown);

private:

    //typedef Hash ResourceMapKey;
    typedef std::string ResourceMapKey;

    std::string                     m_pathAssets;
    std::string                     m_pathScreenshots;
    std::string                     m_defaultFont;
    std::string                     m_debugFont;
    bool                            m_useFullPath; //TODO: some kind of enum RessourceIDPolicy
    bool                            m_defaultTextureSmooth;

    std::map<ResourceMapKey, ResourceInfo*> m_resources;
    std::map<ResourceMapKey, Texture*> m_customTextures;

    std::vector<DelegateDatasheetObjectFactory> m_datasheetObjectFactories;
    std::map<ResourceMapKey, const DatasheetEnum*>  m_datasheetEnums;

    struct ResourceListener
    {
        const void* handle;
        DelegateResourceUpdated delegateResourceUpdated;
    };
    std::map<const Resource*, std::vector<ResourceListener>> m_resourceListeners;
};

ManagerResources* GetResources();

}   // namespace gugu
