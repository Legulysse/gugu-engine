#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Texture;
    class ImageSet;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class SubImage
{
public:

    SubImage(ImageSet* imageSet);
    SubImage(ImageSet* imageSet, const std::string& name, const sf::IntRect& rect);
    ~SubImage();

    ImageSet*       GetImageSet() const;

    void            SetName(const std::string& name);
    bool            IsName(const std::string& name) const;
    const std::string& GetName() const;

    void            SetRect(const sf::IntRect& _oRect);
    const sf::IntRect& GetRect() const;

protected:

    ImageSet*       m_imageSet;

    std::string     m_name;
    sf::IntRect     m_rect;
};

class ImageSet : public Resource
{
public:

    ImageSet    ();
    ~ImageSet   ();

    void        SetTexture(Texture* _pTexture);
    Texture*    GetTexture() const;
    
    SubImage*   AddSubImage     (const std::string& _strName);

    bool        DeleteSubImage  (SubImage* _pNewSubImage);
    void        DeleteAllSubImages();

    SubImage*   GetSubImage         (size_t _uiIndex) const;
    SubImage*   GetSubImage         (const std::string& _strName) const;

    const std::vector<SubImage*>& GetSubImages() const;
    size_t GetSubImageCount() const;

    virtual EResourceType::Type GetResourceType() const override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    Texture*                m_texture;
    std::vector<SubImage*>  m_subImages;
};

}   // namespace gugu
