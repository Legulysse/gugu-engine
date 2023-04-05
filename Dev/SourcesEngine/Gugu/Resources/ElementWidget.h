#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Math/Vector2.h"
#include "Gugu/Math/UDim.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class Texture;
    class ImageSet;
    class SubImage;
}

namespace pugi
{
    class xml_document;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementData
{
public:

    std::vector<ElementData*> children;

    Vector2f origin;
    Vector2f position;
    Vector2f size;
    float rotation = 0.f;
    bool flipV = false;
    bool flipH = false;

    bool useDimOrigin = false;
    bool useDimPosition = false;
    bool useDimSize = false;
    UDim2 dimOrigin;
    UDim2 dimPosition;
    UDim2 dimSize;

    virtual ~ElementData();

    bool LoadFromXml(ElementParseContext& context);
    bool SaveToXml(ElementSaveContext& context) const;

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context);
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const;
};

class ElementSpriteBaseData : public ElementData
{
public:

    sf::IntRect textureRect;
    sf::Color color = sf::Color::White;
    bool repeatTexture = false;
    bool flipTextureV = false;
    bool flipTextureH = false;

    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;
};

class ElementSpriteData : public ElementSpriteBaseData
{
public:

    Texture* texture = nullptr;

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;
};

class ElementSpriteGroupItemData : public ElementSpriteBaseData
{
public:

    SubImage* subImage = nullptr;

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;
};

class ElementSpriteGroupData : public ElementData
{
public:

    ImageSet* imageSet = nullptr;
    Texture* texture = nullptr;
    std::vector<ElementSpriteGroupItemData*> components;

    virtual ~ElementSpriteGroupData();

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;
};

class ElementWidget : public Resource
{
public:

    ElementWidget();
    virtual ~ElementWidget();
    
    Element* InstanciateWidget() const;
    ElementData* GetData() const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    ElementData* m_data;
};

}   // namespace gugu
