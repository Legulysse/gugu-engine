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
#include "Gugu/Resources/Sound.h"
#include "Gugu/Resources/Music.h"
#include "Gugu/Resources/SoundCue.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Resources/Datasheet.h"

#include "Gugu/System/SystemUtility.h"

#include "Gugu/Core/Application.h"
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

void ManagerResources::Release()
{
    m_datasheetObjectFactories.clear();

    ClearStdMap(m_datasheetEnums);
    ClearStdMap(m_customTextures);
    ClearStdMap(m_resources);
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

bool ManagerResources::IsResourceLoaded(const std::string& _strName) const
{
    auto iteElement = m_resources.find(_strName);
    if (iteElement != m_resources.end())
    {
        return (*iteElement).second->resource != nullptr;
    }

    return false;
}

bool ManagerResources::GetResourceFileInfo(const std::string& _strName, FileInfo& fileInfo) const
{
    auto iteElement = m_resources.find(_strName);
    if (iteElement != m_resources.end())
    {
        fileInfo = (*iteElement).second->fileInfo;
        return true;
    }
    
    return false;
}

bool ManagerResources::GetResourcePathName(const std::string& _strName, std::string& pathName) const
{
    auto iteElement = m_resources.find(_strName);
    if (iteElement != m_resources.end())
    {
        pathName = (*iteElement).second->fileInfo.GetPathName();
        return true;
    }

    return false;
}

FileInfo ManagerResources::GetResourceFileInfo(const std::string& _strName) const
{
    FileInfo fileInfo;
    GetResourceFileInfo(_strName, fileInfo);
    return fileInfo;
}

std::string ManagerResources::GetResourcePathName(const std::string& _strName) const
{
    std::string pathName;
    GetResourcePathName(_strName, pathName);
    return pathName;
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
    if (fileInfo.GetExtension() == "png" || fileInfo.GetExtension() == "jpg")
    {
        return EResourceType::Texture;
    }
    else if (fileInfo.GetExtension() == "ttf")
    {
        return EResourceType::Font;
    }
    //else if (fileInfo.GetExtension() == "sound.xml")  // TODO: sound and music files are basically the same thing, I cant deduce them from file extension alone.
    //{
    //    return EResourceType::Sound;
    //}
    //else if (fileInfo.GetExtension() == "music.xml")
    //{
    //    return EResourceType::Music;
    //}
    else if (fileInfo.GetExtension() == "sound.xml" || fileInfo.GetExtension() == "sound")
    {
        return EResourceType::SoundCue;
    }
    else if (fileInfo.GetExtension() == "imageset.xml" || fileInfo.GetExtension() == "imageset")
    {
        return EResourceType::ImageSet;
    }
    else if (fileInfo.GetExtension() == "animset.xml" || fileInfo.GetExtension() == "animset")
    {
        return EResourceType::AnimSet;
    }
    //else if ()
    //{
    //    //TODO: check if the extension is a datasheet
    //}
    else
    {
        return EResourceType::Unknown;
    }
}

Texture* ManagerResources::GetTexture(const std::string& _strName)
{
    return dynamic_cast<Texture*>(GetResource(_strName, EResourceType::Texture));
}

Font* ManagerResources::GetFont(const std::string& _strName)
{
    return dynamic_cast<Font*>(GetResource(_strName, EResourceType::Font));
}

Sound* ManagerResources::GetSound(const std::string& _strName)
{
    return dynamic_cast<Sound*>(GetResource(_strName, EResourceType::Sound));
}

Music* ManagerResources::GetMusic(const std::string& _strName)
{
    return dynamic_cast<Music*>(GetResource(_strName, EResourceType::Music));
}

SoundCue* ManagerResources::GetSoundCue(const std::string& _strName)
{
    return dynamic_cast<SoundCue*>(GetResource(_strName, EResourceType::SoundCue));
}

ImageSet* ManagerResources::GetImageSet(const std::string& _strName)
{
    return dynamic_cast<ImageSet*>(GetResource(_strName, EResourceType::ImageSet));
}

AnimSet* ManagerResources::GetAnimSet(const std::string& _strName)
{
    return dynamic_cast<AnimSet*>(GetResource(_strName, EResourceType::AnimSet));
}

Datasheet* ManagerResources::GetDatasheet(const std::string& _strName)
{
    return dynamic_cast<Datasheet*>(GetResource(_strName, EResourceType::Datasheet));
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

bool ManagerResources::LoadResource(const std::string& _strName, EResourceType::Type _eExplicitType)
{
    auto iteElement = m_resources.find(_strName);
    if (iteElement == m_resources.end())
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource failed, unknown resource : {0}", _strName));
        return false;
    }

    return LoadResource(iteElement->second, _eExplicitType) != nullptr;
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

    if (_eExplicitType == EResourceType::Unknown)
    {
        _eExplicitType = GetResourceType(oFileInfo);
    }

    if (_eExplicitType == EResourceType::Texture)
    {
        pResource = new Texture;
    }
    else if (_eExplicitType == EResourceType::Font)
    {
        pResource = new Font;
    }
    else if (_eExplicitType == EResourceType::Sound)
    {
        pResource = new Sound;
    }
    else if (_eExplicitType == EResourceType::Music)
    {
        pResource = new Music;
    }
    else if (_eExplicitType == EResourceType::SoundCue)
    {
        pResource = new SoundCue;
    }
    else if (_eExplicitType == EResourceType::ImageSet)
    {
        pResource = new ImageSet;
    }
    else if (_eExplicitType == EResourceType::AnimSet)
    {
        pResource = new AnimSet;
    }
    else if (_eExplicitType == EResourceType::Datasheet)
    {
        pResource = new Datasheet;
    }
    else
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource failed, unknown resource extension : {0}", oFileInfo.GetName()));
    }

    if (pResource)
    {
        _pResourceInfo->resource = pResource;
        pResource->Init(_pResourceInfo);
        pResource->LoadFromFile();

        GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Resource loaded : {0}", oFileInfo.GetName()));
    }

    return pResource;
}

bool ManagerResources::InjectResource(const std::string& _strResourceID, Resource* _pResource)
{
    ResourceMapKey mapKey(_strResourceID);

    auto iteAsset = m_resources.find(mapKey);
    if (iteAsset != m_resources.end())
    {
        if (iteAsset->second->resource == nullptr)
        {
            iteAsset->second->resource = _pResource;
            _pResource->Init(iteAsset->second);

            GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Injected Resource : {0}", _strResourceID));
            return true;
        }
        else
        {
            GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("InjectResource failed, the resource is already loaded : {0}", _strResourceID));
        }
    }
    else
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("InjectResource failed, the resource is unknown : {0}", _strResourceID));
    }

    return false;
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

bool ManagerResources::RegisterResourceInfo(const std::string& _strResourceID, const FileInfo& _kFileInfos)
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

void ManagerResources::RemoveResourcesFromPath(const std::string& _strPath)
{
    auto it = m_resources.begin();
    while (it != m_resources.end())
    {
        auto iteResource = it;
        ++it;

        if (StdStringStartsWith(iteResource->second->fileInfo.GetPath(), _strPath))
        {
            ResourceInfo* pInfo = iteResource->second;
            m_resources.erase(iteResource);

            SafeDelete(pInfo);
        }
    }
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

//void ManagerResources::GetLoadedTextureInfos(std::vector<const ResourceInfo*>& _vecInfos) const
//{
//    GetLoadedResourceInfos(_vecInfos, EResourceType::Texture);
//}
//
//void ManagerResources::GetLoadedImageSetInfos(std::vector<const ResourceInfo*>& _vecInfos) const
//{
//    GetLoadedResourceInfos(_vecInfos, EResourceType::ImageSet);
//}
//
//void ManagerResources::GetLoadedAnimSetInfos(std::vector<const ResourceInfo*>& _vecInfos) const
//{
//    GetLoadedResourceInfos(_vecInfos, EResourceType::AnimSet);
//}
//
//void ManagerResources::GetLoadedResourceInfos(std::vector<const ResourceInfo*>& _vecInfos, EResourceType::Type _eType) const
//{
//    auto iteCurrent = m_resources.begin();
//    while (iteCurrent != m_resources.end())
//    {
//        if (iteCurrent->second->resource && (_eType == EResourceType::Unknown || _eType == iteCurrent->second->resource->GetResourceType()))
//            _vecInfos.push_back(iteCurrent->second);
//        ++iteCurrent;
//    }
//
//    std::sort(_vecInfos.begin(), _vecInfos.end(), ResourceInfo::CompareID);
//}

void ManagerResources::GetAllResourceInfos(std::vector<const ResourceInfo*>& _vecInfos) const
{
    _vecInfos.clear();
    _vecInfos.reserve(m_resources.size());

    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        _vecInfos.push_back(iteCurrent->second);
        ++iteCurrent;
    }
}

//void ManagerResources::GetResourceInfosFromPath(std::vector<const ResourceInfo*>& _vecInfos, const std::string& _strPath, EResourceType::Type _eType) const
//{
//    std::string strPathNormalized = NormalizePath(_strPath, true);
//
//    auto iteCurrent = m_resources.begin();
//    while (iteCurrent != m_resources.end())
//    {
//        if (iteCurrent->second->resource && iteCurrent->second->fileInfo.IsPathEnd(strPathNormalized) && (_eType == EResourceType::Unknown || _eType == iteCurrent->second->resource->GetResourceType()))
//            _vecInfos.push_back(iteCurrent->second);
//        ++iteCurrent;
//    }
//
//    std::sort(_vecInfos.begin(), _vecInfos.end(), ResourceInfo::CompareID);
//}

void ManagerResources::RegisterDatasheetObjectFactory(DelegateDatasheetObjectFactory delegateDatasheetObjectFactory)
{
    m_datasheetObjectFactories.push_back(delegateDatasheetObjectFactory);
}

DatasheetObject* ManagerResources::InstanciateDatasheetObject(const std::string& _strType)
{
    if (m_datasheetObjectFactories.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "No Datasheet Object Factory registered : Parsing Datasheets is ignored");
    }
    else
    {
        DatasheetObject* datasheetObject = nullptr;
        for (size_t i = 0; i < m_datasheetObjectFactories.size(); ++i)
        {
            datasheetObject = m_datasheetObjectFactories[i](_strType);
            if (datasheetObject)
            {
                return datasheetObject;
            }
        }
    }

    return nullptr;
}

void ManagerResources::RegisterDatasheetEnum(const std::string& _strName, const DatasheetEnum* _pEnum)
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

const DatasheetEnum* ManagerResources::GetDatasheetEnum(const std::string& _strName)
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
