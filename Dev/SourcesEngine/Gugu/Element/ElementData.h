#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"
#include "Gugu/Math/UDim.h"
#include "Gugu/Element/2D/ElementText.h"    // TODO: this is only used for an enum declaration, find a better alternative.

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct ElementParseContext;
    struct ElementSaveContext;
    class Resource;
    class Texture;
    class ImageSet;
    class SubImage;
    class Font;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class BaseElementData
{
public:

    std::string name;
    BaseElementData* parent = nullptr;
    std::vector<BaseElementData*> children;

    virtual ~BaseElementData();

    bool LoadFromXml(ElementParseContext& context);
    bool SaveToXml(ElementSaveContext& context) const;

    virtual const std::string& GetSerializedType() const = 0;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) = 0;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const = 0;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const = 0;
};

class ElementWidgetInstanceData : public BaseElementData
{
public:

    ElementWidget* widget = nullptr;

    bool overrideOrigin = false;
    bool overridePosition = false;
    bool overrideSize = false;
    bool overrideRotation = false;
    bool overrideFlipV = false;
    bool overrideFlipH = false;

    UDim2 dimOrigin;
    UDim2 dimPosition;
    UDim2 dimSize;

    float rotation = 0.f;
    bool flipV = false;
    bool flipH = false;

    virtual const std::string& GetSerializedType() const override;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
};

class ElementData : public BaseElementData
{
public:

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

    virtual const std::string& GetSerializedType() const override;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
};

class ElementCompositeData : public ElementData
{
public:

    std::vector<BaseElementData*> components;

    virtual ~ElementCompositeData();

    virtual void RefreshCache();

    virtual const std::string& GetSerializedType() const = 0;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
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

    virtual void GetDependencies(std::set<Resource*>& dependencies) const;
};

class ElementSpriteData : public ElementSpriteBaseData
{
public:

    ImageSet* imageSet = nullptr;
    std::string subImageName;
    Texture* texture = nullptr;

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const;
};

class ElementSpriteGroupItemData : public ElementSpriteBaseData
{
public:

    std::string subImageName;

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const;
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

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
};

class ElementTextData : public ElementData
{
public:

    Font* font = nullptr;
    std::string text;
    ETextResizeRule::Type resizeRule = ETextResizeRule::FitSize;
    bool multiline = false;

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const;
};

class ElementButtonData : public ElementData
{
public:

    std::vector<BaseElementData*> components;

    // Cache
    BaseElementData* commonComponent = nullptr;
    BaseElementData* idleStateComponent = nullptr;
    BaseElementData* focusedStateComponent = nullptr;
    BaseElementData* disabledStateComponent = nullptr;

    virtual ~ElementButtonData();

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;

    void RefreshCache();
};

}   // namespace gugu
