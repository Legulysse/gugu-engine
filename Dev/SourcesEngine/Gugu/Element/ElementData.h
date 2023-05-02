#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"
#include "Gugu/Math/UDim.h"

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
    class Texture;
    class ImageSet;
    class SubImage;
    class Font;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementData
{
public:

    std::string name;
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

    virtual void DeepCopy(const ElementData* copyFrom);
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

    virtual void DeepCopy(const ElementData* copyFrom) override;
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

    virtual void DeepCopy(const ElementData* copyFrom) override;
};

class ElementSpriteGroupItemData : public ElementSpriteBaseData
{
public:

    std::string subImageName;

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void DeepCopy(const ElementData* copyFrom) override;
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

    virtual void DeepCopy(const ElementData* copyFrom) override;
};

class ElementTextData : public ElementData
{
public:

    Font* font = nullptr;
    std::string text;
    //ETextResizeRule::Type resizeRule;
    bool multiline = false;

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void DeepCopy(const ElementData* copyFrom) override;
};

class ElementButtonData : public ElementData
{
public:

    std::vector<ElementData*> components;

    // Cache
    ElementData* commonComponent = nullptr;
    ElementData* idleStateComponent = nullptr;
    ElementData* focusedStateComponent = nullptr;
    ElementData* disabledStateComponent = nullptr;

    virtual ~ElementButtonData();

    virtual const std::string& GetSerializedType() const;
    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual void DeepCopy(const ElementData* copyFrom) override;

    void RefreshCache();
};

}   // namespace gugu
