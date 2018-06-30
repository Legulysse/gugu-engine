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

    void            SetName(const std::string& _strName);
    std::string     GetName() const;

    void	        SetRect(const sf::IntRect& _oRect);
    sf::IntRect	    GetRect() const;

protected:

    ImageSet*		m_imageSet;

    std::string     m_name;
    sf::IntRect	    m_rect;
};

class ImageSet : public Resource
{
public:

    ImageSet    ();
    ~ImageSet   ();

    void        SetTexture(Texture* _pTexture);
    Texture*	GetTexture() const;
    
    SubImage*   AddSubImage     (const std::string& _strName);

    bool        DeleteSubImage  (SubImage* _pNewSubImage);
    void        DeleteAllSubImages();

    SubImage*   GetSubImage         (size_t _uiIndex) const;
    SubImage*   GetSubImage         (const std::string& _strName) const;
    void        GetSubImages        (std::vector<SubImage*>& _vecSubImages) const;
    size_t      GetSubImageCount   () const;

    virtual EResourceType::Type	GetResourceType() const override;

    virtual bool    LoadFromFile() override;
    virtual bool    SaveToFile() override;

protected:

    Texture*				m_texture;
    std::vector<SubImage*>	m_subImages;
};

}	// namespace gugu
