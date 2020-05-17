////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Manager/ManagerResources.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"

#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/Resources/Resource.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/Resources/Sound.h"
#include "Gugu/Resources/Music.h"
#include "Gugu/Resources/SoundCue.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Resources/Datasheet.h"

#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Delegate.h"

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/Logger.h"

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
}

ManagerResources::~ManagerResources()
{
}

void ManagerResources::Init(const EngineConfig& config)
{
    m_pathAssets = config.pathAssets;
    m_pathScreenshots = config.pathScreenshots;
    m_useFullPath = config.useAssetsFullPaths;
    m_defaultFont = config.defaultFont;
    m_debugFont = config.debugFont;
    m_defaultTextureSmooth = config.defaultTextureSmooth;

    ParseDirectory(m_pathAssets);
}

void ManagerResources::ParseDirectory(const std::string& _strPathRoot)
{
    //Parse Assets
    GetLogEngine()->Print(ELog::Info, ELogEngine::Resources, "Parsing Resources...");
    GetLogEngine()->Print(ELog::Info, ELogEngine::Resources, StringFormat("Root folder : {0}", _strPathRoot));

    std::vector<FileInfo> vecFiles;
    GetFilesList(_strPathRoot, vecFiles, true);

    int iCount = 0;
    for (size_t i = 0; i < vecFiles.size(); ++i)
    {
        const FileInfo& kFileInfos = vecFiles[i];
        std::string strResourceID = (!m_useFullPath) ? kFileInfos.GetName() : kFileInfos.GetPathName().substr(_strPathRoot.length());

        if (RegisterResourceInfo(strResourceID, kFileInfos))
        {
            ++iCount;
        }
    }

    GetLogEngine()->Print(ELog::Info, ELogEngine::Resources, StringFormat("Finished Parsing Resources (Found {0})", iCount));
}

void ManagerResources::Release()
{
    ClearStdVector(m_datasheetFactories);
    ClearStdMap(m_datasheetEnums);
    ClearStdMap(m_customTextures);
    ClearStdMap(m_resources);
}

std::string ManagerResources::GetPathAssets() const
{
    return m_pathAssets;
}

std::string ManagerResources::GetPathScreenshots() const
{
    return m_pathScreenshots;
}

bool ManagerResources::HasResource(const std::string& _strName) const
{
    auto iteElement = m_resources.find(_strName);
    return (iteElement != m_resources.end());
}

bool ManagerResources::HasResource(const std::string& _strName, FileInfo& _oFileInfo) const
{
    auto iteElement = m_resources.find(_strName);
    if (iteElement != m_resources.end())
    {
        _oFileInfo = (*iteElement).second->fileInfo;
        return true;
    }

    return false;
}

bool ManagerResources::IsResourceLoaded(const std::string& _strName) const
{
    auto iteElement = m_resources.find(_strName);
    if (iteElement != m_resources.end())
    {
        return (*iteElement).second->resource != nullptr;
    }

    return false;
}

bool ManagerResources::IsResourceLoaded(const std::string& _strName, FileInfo& _oFileInfo) const
{
    auto iteElement = m_resources.find(_strName);
    if (iteElement != m_resources.end())
    {
        _oFileInfo = (*iteElement).second->fileInfo;
        return (*iteElement).second->resource != nullptr;
    }

    return false;
}

FileInfo ManagerResources::GetResourceFileInfo(const std::string& _strName) const
{
    auto iteElement = m_resources.find(_strName);
    if (iteElement != m_resources.end())
    {
        return (*iteElement).second->fileInfo;
    }

    return FileInfo();
}

std::string ManagerResources::GetResourcePathName(const std::string& _strName) const
{
    return GetResourceFileInfo(_strName).GetPathName();
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

Texture* ManagerResources::GetTexture(const std::string& _strName)
{
    Resource* pResource = GetResource(_strName, EResourceType::Texture);
    if (pResource)
    {
        return (dynamic_cast<Texture*>(pResource));
    }

    return nullptr;
}

Font* ManagerResources::GetFont(const std::string& _strName)
{
    Resource* pResource = GetResource(_strName, EResourceType::Font);
    if (pResource)
    {
        return (dynamic_cast<Font*>(pResource));
    }

    return nullptr;
}

Sound* ManagerResources::GetSound(const std::string& _strName)
{
    Resource* pResource = GetResource(_strName, EResourceType::Sound);
    if (pResource)
    {
        return (dynamic_cast<Sound*>(pResource));
    }

    return nullptr;
}

Music* ManagerResources::GetMusic(const std::string& _strName)
{
    Resource* pResource = GetResource(_strName, EResourceType::Music);
    if (pResource)
    {
        return (dynamic_cast<Music*>(pResource));
    }

    return nullptr;
}

SoundCue* ManagerResources::GetSoundCue(const std::string& _strName)
{
    Resource* pResource = GetResource(_strName, EResourceType::SoundCue);
    if (pResource)
    {
        return (dynamic_cast<SoundCue*>(pResource));
    }

    return nullptr;
}

ImageSet* ManagerResources::GetImageSet(const std::string& _strName)
{
    Resource* pResource = GetResource(_strName, EResourceType::ImageSet);
    if (pResource)
    {
        return (dynamic_cast<ImageSet*>(pResource));
    }

    return nullptr;
}

AnimSet* ManagerResources::GetAnimSet(const std::string& _strName)
{
    Resource* pResource = GetResource(_strName, EResourceType::AnimSet);
    if (pResource)
    {
        return (dynamic_cast<AnimSet*>(pResource));
    }

    return nullptr;
}

Datasheet* ManagerResources::GetDatasheet(const std::string& _strName)
{
    Resource* pResource = GetResource(_strName, EResourceType::Datasheet);
    if (pResource)
    {
        return (dynamic_cast<Datasheet*>(pResource));
    }

    return nullptr;
}

Resource* ManagerResources::GetResource(const std::string& _strName, EResourceType::Type _eExplicitType)
{
    //Check Resource already loaded
    auto iteResource = m_resources.find(_strName);
    if (iteResource != m_resources.end())
    {
        if (iteResource->second->resource)
            return iteResource->second->resource;

        Resource* pResource = LoadResource(iteResource->second, _eExplicitType);
        return pResource;
    }

    GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("GetResource failed, unknown resource : {0}", _strName));
    return nullptr;
}

Resource* ManagerResources::LoadResource(const std::string& _strName, EResourceType::Type _eExplicitType)
{
    auto iteElement = m_resources.find(_strName);
    if (iteElement == m_resources.end())
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource failed, unknown resource : {0}", _strName));
        return nullptr;
    }

    return LoadResource(iteElement->second, _eExplicitType);
}

Resource* ManagerResources::LoadResource(ResourceInfo* _pResourceInfo, EResourceType::Type _eExplicitType)
{
    if (_pResourceInfo->resource)
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource ignored, resource already loaded : {0}", _pResourceInfo->fileInfo.GetName()));
        return _pResourceInfo->resource;
    }

    FileInfo oFileInfo = _pResourceInfo->fileInfo;
    Resource* pResource = nullptr;

    if (_eExplicitType == EResourceType::Texture || oFileInfo.GetExtension() == "png" || oFileInfo.GetExtension() == "jpg")
    {
        pResource = new Texture;
    }
    else if (_eExplicitType == EResourceType::Font || oFileInfo.GetExtension() == "ttf")
    {
        pResource = new Font;
    }
    else if (_eExplicitType == EResourceType::Sound || oFileInfo.GetExtension() == "sound.xml")
    {
        pResource = new Sound;
    }
    else if (_eExplicitType == EResourceType::Music || oFileInfo.GetExtension() == "music.xml")
    {
        pResource = new Music;
    }
    else if (_eExplicitType == EResourceType::SoundCue || StdStringEndsWith(oFileInfo.GetExtension(), "sound.xml") || StdStringEndsWith(oFileInfo.GetExtension(), "sound"))
    {
        pResource = new SoundCue;
    }
    else if (_eExplicitType == EResourceType::ImageSet || StdStringEndsWith(oFileInfo.GetExtension(), "imageset.xml") || StdStringEndsWith(oFileInfo.GetExtension(), "imageset"))
    {
        pResource = new ImageSet;
    }
    else if (_eExplicitType == EResourceType::AnimSet || StdStringEndsWith(oFileInfo.GetExtension(), "animset.xml") || StdStringEndsWith(oFileInfo.GetExtension(), "animset"))
    {
        pResource = new AnimSet;
    }
    else if (_eExplicitType == EResourceType::Datasheet)    //TODO: check if the extension is a datasheet
    {
        pResource = InstanciateDatasheet(oFileInfo.GetExtension());
        if (!pResource)
        {
            GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Could not instantiate Datasheet : {0}", oFileInfo.GetName()));
        }
    }
    else
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource failed, unknown resource extension : {0}", oFileInfo.GetName()));
    }

    if (pResource)
    {
        GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Resource loaded : {0}", oFileInfo.GetName()));

        _pResourceInfo->resource = pResource;

        pResource->Init(_pResourceInfo);
        pResource->LoadFromFile();
    }

    return pResource;
}

std::string ManagerResources::GetResourceID(const Resource* _pResource) const
{
    if (!_pResource)
        return "";

    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        if (iteCurrent->second->resource == _pResource)
            return iteCurrent->second->resourceID;
        ++iteCurrent;
    }

    return "";
}

std::string ManagerResources::GetResourceID(const FileInfo& _oFileInfo) const
{
    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        if (iteCurrent->second->fileInfo == _oFileInfo)
            return iteCurrent->second->resourceID;
        ++iteCurrent;
    }

    return "";
}

bool ManagerResources::RegisterResourceInfo(std::string _strResourceID, const FileInfo& _kFileInfos)
{
    ResourceMapKey mapKey(_strResourceID);

    auto iteAsset = m_resources.find(mapKey);
    if (iteAsset == m_resources.end())
    {
        ResourceInfo* pInfo = new ResourceInfo;
        pInfo->resourceID = _strResourceID;
        pInfo->fileInfo = _kFileInfos;
        pInfo->resource = nullptr;

        m_resources.insert(iteAsset, std::make_pair(mapKey, pInfo));
        
        GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Registered Resource : Key/Hash = {0}, ID = {1}, Path = {2}"
            , mapKey
            , _strResourceID
            , _kFileInfos.GetPath()));
        
        return true;
    }
    
    GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("A Resource hashed ID is already registered : Key/Hash = {0}, New ID = {1}, New Path = {2}, Registered ID = {3}, Registered Path = {4}"
        , mapKey
        , _strResourceID
        , _kFileInfos.GetPath()
        , iteAsset->second->resourceID
        , iteAsset->second->fileInfo.GetPath()));
        
    return false;
}

bool ManagerResources::AddResource(Resource* _pNewResource, const FileInfo& _oFileInfo)
{
    if (!_pNewResource)
        return false;

    std::string strPathName = _oFileInfo.GetPathName();
    std::string strName     = _oFileInfo.GetName();

    if (strName.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("AddResource failed, name invalid : {0}", strName));
        return false;
    }

    std::string strResourceID = strName;
    if (m_useFullPath)
        strResourceID = strPathName.substr(m_pathAssets.length());

    auto iteResource = m_resources.find(strResourceID);
    if (iteResource != m_resources.end())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("AddResource failed, Resource already exists : {0}", strName));
        return false;
    }

    ResourceInfo* pInfo = new ResourceInfo;
    pInfo->resourceID = strResourceID;
    pInfo->fileInfo = _oFileInfo;
    pInfo->resource = _pNewResource;

    _pNewResource->Init(pInfo);

    m_resources.insert(iteResource, std::make_pair(strResourceID, pInfo));

    return true;
}

bool ManagerResources::MoveResource(Resource* _pResource, const FileInfo& _oFileInfo)
{
    if (!_pResource)
        return false;

    std::string strNewPathName = _oFileInfo.GetPathName();
    std::string strNewName     = _oFileInfo.GetName();

    if (strNewName.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("RenameResource failed, new name invalid : {0}", strNewName));
        return false;
    }

    std::string strResourceID = strNewName;
    if (m_useFullPath)
        strResourceID = strNewPathName.substr(m_pathAssets.length());

    auto iteNewResource = m_resources.find(strResourceID);
    if (iteNewResource != m_resources.end())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("RenameResource failed, Resource already exists : {0}", strNewName));
        return false;
    }

    FileInfo oFileInfoOld = _pResource->GetFileInfo();

    auto iteOldResource = m_resources.find(_pResource->GetID());
    if (iteOldResource == m_resources.end())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("RenameResource failed, Resource not found : {0}", oFileInfoOld.GetName()));
        return false;
    }

    ResourceInfo* pInfo = iteOldResource->second;
    m_resources.erase(iteOldResource);

    pInfo->resourceID = strResourceID;
    pInfo->fileInfo = _oFileInfo;

    m_resources.insert(iteNewResource, std::make_pair(strResourceID, pInfo));

    //Delete old file, save new file
    if (_pResource->SaveToFile())
    {
        RemoveFile(oFileInfoOld.GetPathName());
        return true;
    }

    return false;
}

bool ManagerResources::RemoveResource(Resource* _pResource)
{
    if (!_pResource)
        return false;

    auto iteResource = m_resources.find(_pResource->GetID());
    if (iteResource != m_resources.end())
    {
        ResourceInfo* pInfo = iteResource->second;
        m_resources.erase(iteResource);

        SafeDelete(pInfo);
    }

    return true;
}

bool ManagerResources::DeleteResource(Resource* _pResource)
{
    if (!_pResource)
        return false;

    FileInfo oFileInfo = _pResource->GetFileInfo();

    if (RemoveResource(_pResource))
    {
        return RemoveFile(oFileInfo.GetPathName());
    }
    return false;
}

Texture* ManagerResources::GetCustomTexture(const std::string& _strName)
{
    auto iteElement = m_customTextures.find(_strName);
    if (iteElement != m_customTextures.end())
    {
        return (iteElement->second);
    }

    sf::Texture* pNewSFTexture = new sf::Texture;

    Texture* pNewTexture = new Texture;
    pNewTexture->SetSFTexture(pNewSFTexture);

    iteElement = m_customTextures.insert(iteElement, std::make_pair(_strName, pNewTexture));
    return pNewTexture;
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

void ManagerResources::GetLoadedTextureInfos(std::vector<const ResourceInfo*>& _vecInfos) const
{
    GetLoadedResourceInfos(_vecInfos, EResourceType::Texture);
}

void ManagerResources::GetLoadedImageSetInfos(std::vector<const ResourceInfo*>& _vecInfos) const
{
    GetLoadedResourceInfos(_vecInfos, EResourceType::ImageSet);
}

void ManagerResources::GetLoadedAnimSetInfos(std::vector<const ResourceInfo*>& _vecInfos) const
{
    GetLoadedResourceInfos(_vecInfos, EResourceType::AnimSet);
}

void ManagerResources::GetLoadedResourceInfos(std::vector<const ResourceInfo*>& _vecInfos, EResourceType::Type _eType) const
{
    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        if (iteCurrent->second->resource && (_eType == EResourceType::Unknown || _eType == iteCurrent->second->resource->GetResourceType()))
            _vecInfos.push_back(iteCurrent->second);
        ++iteCurrent;
    }

    std::sort(_vecInfos.begin(), _vecInfos.end(), ResourceInfo::Compare);
}

void ManagerResources::GetResourceInfos(std::vector<const ResourceInfo*>& _vecInfos) const
{
    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        _vecInfos.push_back(iteCurrent->second);
        ++iteCurrent;
    }

    std::sort(_vecInfos.begin(), _vecInfos.end(), ResourceInfo::Compare);
}

void ManagerResources::GetResourceInfosFromPath(std::vector<const ResourceInfo*>& _vecInfos, const std::string& _strPath, EResourceType::Type _eType) const
{
    std::string strPathNormalized = NormalizePath(_strPath, true);

    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        if (iteCurrent->second->resource && iteCurrent->second->fileInfo.IsPathEnd(strPathNormalized)&& (_eType == EResourceType::Unknown || _eType == iteCurrent->second->resource->GetResourceType()))
            _vecInfos.push_back(iteCurrent->second);
        ++iteCurrent;
    }

    std::sort(_vecInfos.begin(), _vecInfos.end(), ResourceInfo::Compare);
}

void ManagerResources::RegisterDatasheetFactory(DelegateStatic1P<const std::string&, Datasheet*>* _pDatasheetFactory)
{
    m_datasheetFactories.push_back(_pDatasheetFactory);
}

Datasheet* ManagerResources::InstanciateDatasheet(const std::string& _strType)
{
    if (m_datasheetFactories.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "No Datasheet Factory registered : Parsing Datasheets is ignored");
    }
    else
    {
        Datasheet* pDatasheet = nullptr;
        for (size_t i = 0; i < m_datasheetFactories.size(); ++i)
        {
            pDatasheet = m_datasheetFactories[i]->Call(_strType);
            if (pDatasheet)
                return pDatasheet;
        }
    }
    return nullptr;
}

void ManagerResources::RegisterDatasheetEnum(const std::string& _strName, DatasheetEnum* _pEnum)
{
    auto iteElement = m_datasheetEnums.find(_strName);
    if (iteElement != m_datasheetEnums.end())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "Datasheet Enum already registered");
        SafeDelete(_pEnum);
    }
    else
    {
        m_datasheetEnums[_strName] = _pEnum;
    }
}

DatasheetEnum* ManagerResources::GetDatasheetEnum(const std::string& _strName)
{
    auto iteElement = m_datasheetEnums.find(_strName);
    if (iteElement != m_datasheetEnums.end())
    {
        return (iteElement->second);
    }
    return nullptr;
}

ManagerResources* GetResources()
{
    return GetEngine()->GetManagerResources();
}

}   // namespace gugu
