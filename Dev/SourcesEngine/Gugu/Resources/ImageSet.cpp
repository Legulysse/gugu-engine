////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ImageSet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlWrap.h"
#include "Gugu/Resources/Texture.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
SubImage::SubImage(ImageSet* imageSet)
: m_imageSet(imageSet)
{
}

SubImage::SubImage(ImageSet* imageSet, const std::string& name, const sf::IntRect& rect)
: m_imageSet(imageSet)
, m_name(name)
, m_rect(rect)
{
}

SubImage::~SubImage()
{
}

ImageSet* SubImage::GetImageSet() const
{
    return m_imageSet;
}

const std::string& SubImage::GetName() const
{
    return m_name;
}

void SubImage::SetName(const std::string& name)
{
    m_name = name;
}

bool SubImage::IsName(const std::string& name) const
{
    return m_name == name;
}

const sf::Rect<int>& SubImage::GetRect() const
{
    return m_rect;
}

void SubImage::SetRect(const sf::IntRect& _oRect)
{
    m_rect = _oRect;
}


ImageSet::ImageSet()
{
    m_texture = nullptr;
}

ImageSet::~ImageSet()
{
    Unload();
}

void ImageSet::SetTexture(Texture* _pTexture)
{
    m_texture = _pTexture;
}

Texture* ImageSet::GetTexture() const
{
    return m_texture;
}

SubImage* ImageSet::AddSubImage(const std::string& _strName)
{
    SubImage* pSubImage = new SubImage(this);
    m_subImages.push_back(pSubImage);
    pSubImage->SetName(_strName);
    return pSubImage;
}

bool ImageSet::DeleteSubImage(SubImage* _pSubImage)
{
    if (!_pSubImage)
        return false;

    auto iteSubImage = StdVectorFind(m_subImages, _pSubImage);
    if (iteSubImage != m_subImages.end())
    {
        m_subImages.erase(iteSubImage);
        SafeDelete(_pSubImage);
        return true;
    }

    return false;
}

void ImageSet::DeleteAllSubImages()
{
    ClearStdVector(m_subImages);
}

SubImage* ImageSet::GetSubImage(size_t _uiIndex) const
{
    if (_uiIndex >= 0 && _uiIndex < m_subImages.size())
        return m_subImages[_uiIndex];
    return nullptr;
}

SubImage* ImageSet::GetSubImage(const std::string& _strName) const
{
    for (size_t i = 0; i < m_subImages.size(); ++i)
    {
        if (m_subImages[i]->IsName(_strName))
            return m_subImages[i];
    }

    return nullptr;
}

const std::vector<SubImage*>& ImageSet::GetSubImages() const
{
    return m_subImages;
}

size_t ImageSet::GetSubImageCount() const
{
    return m_subImages.size();
}

EResourceType::Type ImageSet::GetResourceType() const
{
    return EResourceType::ImageSet;
}

void ImageSet::GetDependencies(std::set<Resource*>& dependencies) const
{
    if (m_texture)
    {
        dependencies.insert(m_texture);
    }
}

void ImageSet::OnDependencyRemoved(const Resource* removedDependency)
{
    if (m_texture == removedDependency)
    {
        m_texture = nullptr;
    }
}

void ImageSet::Unload()
{
    DeleteAllSubImages();
    m_texture = nullptr;
}

bool ImageSet::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node nodeRoot = document.child("ImageSet");
    if (!nodeRoot)
        return false;

    pugi::xml_attribute oAttributeTexture = nodeRoot.attribute("texture");
    if (oAttributeTexture)
        m_texture = GetResources()->GetTexture(oAttributeTexture.as_string());

    for (pugi::xml_node oNodeSubImage = nodeRoot.child("SubImage"); oNodeSubImage; oNodeSubImage = oNodeSubImage.next_sibling("SubImage"))
    {
        pugi::xml_attribute oAttributeName = oNodeSubImage.attribute("name");
        pugi::xml_attribute oAttributeX = oNodeSubImage.attribute("x");
        pugi::xml_attribute oAttributeY = oNodeSubImage.attribute("y");
        pugi::xml_attribute oAttributeW = oNodeSubImage.attribute("w");
        pugi::xml_attribute oAttributeH = oNodeSubImage.attribute("h");

        //Try to read SubImage
        if (oAttributeName
            && oAttributeX
            && oAttributeY
            && oAttributeW
            && oAttributeH)
        {
            std::string strNameSubImage = oAttributeName.as_string();
            int x = oAttributeX.as_int();
            int y = oAttributeY.as_int();
            int w = oAttributeW.as_int();
            int h = oAttributeH.as_int();

            m_subImages.push_back(new SubImage(this, strNameSubImage, sf::IntRect(x, y, w, h)));
        }
    }

    return true;
}

bool ImageSet::SaveToXml(pugi::xml_document& document) const
{
    pugi::xml_node nodeRoot = document.append_child("ImageSet");
    nodeRoot.append_attribute("serializationVersion") = 1;

    nodeRoot.append_attribute("texture") = (!m_texture) ? "" : m_texture->GetID().c_str();

    for (const SubImage* subImage : m_subImages)
    {
        pugi::xml_node nodeSubImage = nodeRoot.append_child("SubImage");

        sf::IntRect oRect = subImage->GetRect();
        nodeSubImage.append_attribute("name") = subImage->GetName().c_str();
        nodeSubImage.append_attribute("x") = oRect.left;
        nodeSubImage.append_attribute("y") = oRect.top;
        nodeSubImage.append_attribute("w") = oRect.width;
        nodeSubImage.append_attribute("h") = oRect.height;
    }

    return true;
}

}   // namespace gugu
