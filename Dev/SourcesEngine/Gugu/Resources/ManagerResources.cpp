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
#include "Gugu/Resources/ParticleEffect.h"
#include "Gugu/Resources/Datasheet.h"
#include "Gugu/Resources/ElementWidget.h"

#include "Gugu/Data/DataBindingUtility.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/Path.h"
#include "Gugu/System/Platform.h"
#include "Gugu/System/String.h"

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
    m_handleResourceDependencies = false;
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
    m_handleResourceDependencies = config.handleResourceDependencies;

    ParseDirectory(m_pathAssets);
}

void ManagerResources::Release()
{
    m_resourceDependencies.clear();
    m_dataObjectFactories.clear();

    ClearStdMap(m_dataEnumInfos);
    ClearStdMap(m_customTextures);
    ClearStdMap(m_resources);
}

void ManagerResources::ParseDirectory(const std::string& _strPathRoot)
{
    //Parse Assets
    GetLogEngine()->Print(ELog::Info, ELogEngine::Resources, "Parsing Resources...");
    GetLogEngine()->Print(ELog::Info, ELogEngine::Resources, StringFormat("Root directory : {0}", _strPathRoot));

    std::vector<FileInfo> vecFiles;
    GetFiles(_strPathRoot, vecFiles, true);

    int iCount = 0;
    for (size_t i = 0; i < vecFiles.size(); ++i)
    {
        const FileInfo& kFileInfos = vecFiles[i];
        std::string strResourceID = (!m_useFullPath) ? std::string(kFileInfos.GetFileName_utf8()) : std::string(kFileInfos.GetFilePath_utf8().substr(_strPathRoot.length()));

        if (RegisterResourceInfo(strResourceID, kFileInfos))
        {
            ++iCount;
        }
    }

    GetLogEngine()->Print(ELog::Info, ELogEngine::Resources, StringFormat("Finished Parsing Resources (Found {0})", iCount));
}

const std::string& ManagerResources::GetPathAssets() const
{
    return m_pathAssets;
}

const std::string& ManagerResources::GetPathScreenshots() const
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
        // This will return a copy.
        fileInfo = (*iteElement).second->fileInfo;
        return true;
    }

    return false;
}

const FileInfo& ManagerResources::GetResourceFileInfo(const std::string& _strName) const
{
    auto iteElement = m_resources.find(_strName);
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
    //else if (fileInfo.HasExtension("sound.xml"))  // TODO: sound and music files are basically the same thing, I cant deduce them from file extension alone.
    //{
    //    return EResourceType::Sound;
    //}
    //else if (fileInfo.HasExtension("music.xml"))
    //{
    //    return EResourceType::Music;
    //}
    else if (fileInfo.HasExtension("sound.xml") || fileInfo.HasExtension("sound"))
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

ParticleEffect* ManagerResources::GetParticleEffect(const std::string& _strName)
{
    return dynamic_cast<ParticleEffect*>(GetResource(_strName, EResourceType::ParticleEffect));
}

Datasheet* ManagerResources::GetDatasheet(const std::string& _strName)
{
    return dynamic_cast<Datasheet*>(GetResource(_strName, EResourceType::Datasheet));
}

const DatasheetObject* ManagerResources::GetDatasheetRootObject(const std::string& _strName)
{
    Datasheet* sheet = GetDatasheet(_strName);
    return sheet == nullptr ? nullptr : sheet->GetRootObject();
}

ElementWidget* ManagerResources::GetElementWidget(const std::string& _strName)
{
    return dynamic_cast<ElementWidget*>(GetResource(_strName, EResourceType::ElementWidget));
}

Resource* ManagerResources::GetResource(const std::string& _strName, EResourceType::Type _eExplicitType)
{
    if (_strName.empty())
        return nullptr;

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
    if (_strName.empty())
        return false;

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
    if (!_pResourceInfo)
        return nullptr;

    if (_pResourceInfo->resource)
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource ignored, resource already loaded : {0}", _pResourceInfo->resourceID));
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
    else if (_eExplicitType == EResourceType::ParticleEffect)
    {
        pResource = new ParticleEffect;
    }
    else if (_eExplicitType == EResourceType::Datasheet)
    {
        pResource = new Datasheet;
    }
    else if (_eExplicitType == EResourceType::ElementWidget)
    {
        pResource = new ElementWidget;
    }
    else
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("LoadResource failed, unknown resource extension : {0}", oFileInfo.GetFilePath_utf8()));
    }

    if (pResource)
    {
        _pResourceInfo->resource = pResource;
        RegisterResourceDependencies(pResource);

        pResource->Init(_pResourceInfo);
        pResource->LoadFromFile();

        UpdateResourceDependencies(pResource);

        GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Resource loaded : {0}", _pResourceInfo->resourceID));
    }

    return pResource;
}

bool ManagerResources::InjectResource(const std::string& _strResourceID, Resource* _pResource)
{
    if (_strResourceID.empty())
        return false;

    ResourceMapKey mapKey(_strResourceID);

    auto iteAsset = m_resources.find(mapKey);
    if (iteAsset != m_resources.end())
    {
        if (iteAsset->second->resource == nullptr)
        {
            iteAsset->second->resource = _pResource;
            RegisterResourceDependencies(_pResource);

            _pResource->Init(iteAsset->second);
            _pResource->LoadFromFile();

            UpdateResourceDependencies(_pResource);

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

const std::string& ManagerResources::GetResourceID(const Resource* _pResource) const
{
    if (_pResource)
    {
        auto iteCurrent = m_resources.begin();
        while (iteCurrent != m_resources.end())
        {
            if (iteCurrent->second->resource == _pResource)
                return iteCurrent->second->resourceID;
            ++iteCurrent;
        }
    }

    static const std::string defaultValue;
    return defaultValue;
}

const std::string& ManagerResources::GetResourceID(const FileInfo& _oFileInfo) const
{
    // TODO: I should be able to deduce an ID from a FileInfo.
    auto iteCurrent = m_resources.begin();
    while (iteCurrent != m_resources.end())
    {
        if (iteCurrent->second->fileInfo == _oFileInfo)
            return iteCurrent->second->resourceID;
        ++iteCurrent;
    }

    static const std::string defaultValue;
    return defaultValue;
}

bool ManagerResources::RegisterResourceInfo(const std::string& _strResourceID, const FileInfo& _kFileInfos)
{
    if (_strResourceID.empty())
        return false;

    ResourceMapKey mapKey(_strResourceID);

    auto iteAsset = m_resources.find(mapKey);
    if (iteAsset == m_resources.end())
    {
        ResourceInfo* pInfo = new ResourceInfo;
        pInfo->resourceID = _strResourceID;
        pInfo->fileInfo = _kFileInfos;
        pInfo->resource = nullptr;

        // TODO: investigate the use of hint iterator here, seems like a bad usage.
        m_resources.insert(iteAsset, std::make_pair(mapKey, pInfo));
        
        GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Registered Resource : ID = {0}, Path = {1}"
            , _strResourceID
            , _kFileInfos.GetFilePath_utf8()));
        
        return true;
    }
    
    GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("A Resource ID is already registered : ID = {0}, New Path = {1}, Registered Path = {2}"
        , _strResourceID
        , _kFileInfos.GetFilePath_utf8()
        , iteAsset->second->fileInfo.GetFilePath_utf8()));
        
    return false;
}

bool ManagerResources::AddResource(Resource* _pNewResource, const FileInfo& _oFileInfo)
{
    if (!_pNewResource)
        return false;

    std::string strPathName = std::string(_oFileInfo.GetFilePath_utf8());
    std::string strName     = std::string(_oFileInfo.GetFileName_utf8());

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

    m_resources.insert(iteResource, std::make_pair(strResourceID, pInfo));
    RegisterResourceDependencies(_pNewResource);

    _pNewResource->Init(pInfo);

    UpdateResourceDependencies(_pNewResource);

    GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Added Resource : ID = {0}, Path = {1}"
        , strResourceID
        , _oFileInfo.GetFilePath_utf8()));

    return true;
}

bool ManagerResources::MoveResource(Resource* _pResource, const FileInfo& _oFileInfo)
{
    if (!_pResource)
        return false;

    std::string strNewPathName = std::string(_oFileInfo.GetFilePath_utf8());
    std::string strNewName     = std::string(_oFileInfo.GetFileName_utf8());

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
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("RenameResource failed, Resource not found : {0}", oFileInfoOld.GetFilePath_utf8()));
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
        RemoveFile(oFileInfoOld.GetFilePath_utf8());
        return true;
    }

    GetLogEngine()->Print(ELog::Debug, ELogEngine::Resources, StringFormat("Moved Resource : ID = {0}, Path = {1}"
        , strResourceID
        , _oFileInfo.GetFilePath_utf8()));

    return false;
}

bool ManagerResources::RemoveResource(Resource* _pResource)
{
    if (!_pResource)
        return false;

    return RemoveResource(_pResource->GetID());
}

bool ManagerResources::RemoveResource(const std::string& resourceID)
{
    auto iteResource = m_resources.find(resourceID);
    if (iteResource != m_resources.end())
    {
        Resource* removedResource = iteResource->second->resource;

        ResourceInfo* pInfo = iteResource->second;
        m_resources.erase(iteResource);

        if (removedResource)
        {
            NotifyResourceRemoved(removedResource);
            UnregisterResourceDependencies(removedResource);
        }

        SafeDelete(pInfo);
    }

    return true;
}

bool ManagerResources::DeleteResource(Resource* _pResource)
{
    if (!_pResource)
        return false;

    return DeleteResource(_pResource->GetID());
}

bool ManagerResources::DeleteResource(const std::string& resourceID)
{
    FileInfo fileInfo;
    if (GetResourceFileInfo(resourceID, fileInfo))
    {
        if (RemoveResource(resourceID))
        {
            return RemoveFile(fileInfo.GetFilePath_utf8());
        }
    }

    return false;
}

void ManagerResources::RemoveResourcesFromPath(const std::string& _strPath)
{
    if (_strPath.empty())
        return;

    auto it = m_resources.begin();
    while (it != m_resources.end())
    {
        auto iteResource = it;
        ++it;

        if (PathStartsWith(iteResource->second->fileInfo.GetDirectoryPath_utf8(), _strPath))
        {
            RemoveResource(iteResource->first);
        }
    }
}

Texture* ManagerResources::GetCustomTexture(const std::string& _strName)
{
    if (_strName.empty())
        return nullptr;

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

void ManagerResources::RegisterDataEnumInfos(const std::string& _strName, const DataEnumInfos* _pEnum)
{
    auto iteElement = m_dataEnumInfos.find(_strName);
    if (iteElement != m_dataEnumInfos.end())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "Data Enum already registered");
        SafeDelete(_pEnum);
    }
    else
    {
        m_dataEnumInfos[_strName] = _pEnum;
    }
}

const DataEnumInfos* ManagerResources::GetDataEnumInfos(const std::string& _strName)
{
    auto iteElement = m_dataEnumInfos.find(_strName);
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

bool ManagerResources::RegisterResourceListener(const Resource* resource, const void* handle, const DelegateResourceEvent& delegateResourceEvent)
{
    if (!resource || !handle)
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

void ManagerResources::UnregisterResourceListeners(const Resource* resource, const void* handle)
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

void ManagerResources::UnregisterResourceListeners(const void* handle)
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
