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
    class Datasheet;
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

    using DelegateDatasheetFactory = std::function<Datasheet* (const std::string&)>;

public:

    ManagerResources();
    ~ManagerResources();

    void Init(const EngineConfig& config);
    void Release();

    std::string GetPathAssets       () const;
    std::string GetPathScreenshots  () const;
    
    bool        HasResource         (const std::string& _strName) const;
    bool        HasResource         (const std::string& _strName, FileInfo& _oFileInfo) const;
    bool        IsResourceLoaded    (const std::string& _strName) const;
    bool        IsResourceLoaded    (const std::string& _strName, FileInfo& _oFileInfo) const;

    FileInfo    GetResourceFileInfo (const std::string& _strName) const;
    std::string GetResourcePathName (const std::string& _strName) const;

    void        ParseDirectory  (const std::string& _strPath);
    void        PreloadAll      ();
    void        SaveAll         ();

    Texture*    GetTexture      (const std::string& _strName);
    Font*       GetFont         (const std::string& _strName);
    Sound*      GetSound        (const std::string& _strName);
    Music*      GetMusic        (const std::string& _strName);
    SoundCue*   GetSoundCue     (const std::string& _strName);
    ImageSet*   GetImageSet     (const std::string& _strName);
    AnimSet*    GetAnimSet      (const std::string& _strName);
    Datasheet*  GetDatasheet    (const std::string& _strName);

    template<typename T>
    T*          GetDatasheet    (const std::string& _strName)
    {
        return static_cast<T*>(GetDatasheet(_strName));
    }

    Resource*   GetResource     (const std::string& _strName, EResourceType::Type _eExplicitType = EResourceType::Unknown); //TODO: Whats the difference with LoadResource ? make this more clear
    Resource*   LoadResource    (const std::string& _strName, EResourceType::Type _eExplicitType = EResourceType::Unknown);

    std::string GetResourceID   (const Resource* _pResource) const;
    std::string GetResourceID   (const FileInfo& _oFileInfo) const;

    bool        RegisterResourceInfo(std::string _strResourceID, const FileInfo& _kFileInfos);
    bool        AddResource     (Resource* _pNewResource, const FileInfo& _oFileInfo);
    bool        MoveResource    (Resource* _pResource, const FileInfo& _oFileInfo);
    bool        RemoveResource  (Resource* _pResource);
    bool        DeleteResource  (Resource* _pResource);

    void        GetAllResourceInfos(std::vector<const ResourceInfo*>& _vecInfos) const;

    // TODO: Obsolete editor getters ?
    void        GetLoadedResourceInfos  (std::vector<const ResourceInfo*>& _vecInfos, EResourceType::Type _eType = EResourceType::Unknown) const;
    void        GetLoadedImageSetInfos  (std::vector<const ResourceInfo*>& _vecInfos) const;
    void        GetLoadedAnimSetInfos   (std::vector<const ResourceInfo*>& _vecInfos) const;
    void        GetLoadedTextureInfos   (std::vector<const ResourceInfo*>& _vecInfos) const;
    void        GetResourceInfosFromPath(std::vector<const ResourceInfo*>& _vecInfos, const std::string& _strPath, EResourceType::Type _eType = EResourceType::Unknown) const;

    //TODO: Refactor with ResourceContext
    Texture*        GetCustomTexture(const std::string& _strName);

    bool            IsDefaultTextureSmooth() const;

    Font*           GetDefaultFont();
    Font*           GetDebugFont();
    
    void                RegisterDatasheetFactory    (DelegateDatasheetFactory delegateDatasheetFactory);
    Datasheet*          InstanciateDatasheet        (const std::string& _strType);

    void                RegisterDatasheetEnum       (const std::string& _strName, DatasheetEnum* _pEnum);
    DatasheetEnum*      GetDatasheetEnum            (const std::string& _strName);

private:

    Resource*   LoadResource    (ResourceInfo* _pResourceInfo, EResourceType::Type _eExplicitType = EResourceType::Unknown);

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

    std::vector<DelegateDatasheetFactory> m_datasheetFactories;
    std::map<ResourceMapKey, DatasheetEnum*>  m_datasheetEnums;
};

ManagerResources* GetResources();

}   // namespace gugu
