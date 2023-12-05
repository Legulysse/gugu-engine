#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"
#include "Gugu/Math/UDim.h"
#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Transformable.hpp>

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct RenderPass;
    struct InteractionInfos;
    struct ElementDataContext;
    class ElementEventHandler;
    class ElementWidget;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class Element
{
public:

    Element();
    virtual ~Element();

    //----------------------------------------------
    // Hierarchy

    template<typename T>
    T* AddChild()
    {
        T* child = new T;
        AddChild(child);
        return child;
    }

    void AddChild(Element* child);
    bool InsertChild(Element* child, size_t index);
    Element* AddChildWidget(const std::string& elementWidgetID);
    Element* AddChildWidget(ElementWidget* elementWidget);

    void RemoveChild(Element* child);
    void DeleteAllChildren();

    // Set the Element's parent, without registering it in the parent's children list.
    void SetParent(Element* parent);

    Element* GetParent() const;
    Element* GetTopParent() const;

    size_t GetChildCount() const;
    const std::vector<Element*>& GetChildren() const;

    //----------------------------------------------
    // Position

    void SetPositionX(float _fPosX);
    void SetPositionY(float _fPosY);
    void SetPosition(float _fPosX, float _fPosY);
    void SetPosition(const Vector2f& _kPosition);

    void Move(float _fOffsetX, float _fOffsetY);
    void Move(const Vector2f& _kOffset);

    const Vector2f& GetPosition() const;

    //----------------------------------------------
    // Rotation

    void SetRotation(float _fAngle);
    void Rotate(float _fAngle);

    float GetRotation() const;

    //----------------------------------------------
    // Scale

    void SetScaleX(float _fScaleX);
    void SetScaleY(float _fScaleY);
    void SetScale(float _fScale);
    void SetScale(float _fScaleX, float _fScaleY);
    void SetScale(const Vector2f& _kScale);

    void Scale(float factorX, float factorY);
    void Scale(const Vector2f& factor);

    Vector2f GetScale() const;

    //----------------------------------------------
    // Origin

    void SetOriginX(float _fOriginX);
    void SetOriginY(float _fOriginY);
    void SetOrigin(float _fOriginX, float _fOriginY);
    void SetOrigin(const Vector2f& _kOrigin);

    const Vector2f& GetOrigin() const;

    //----------------------------------------------
    // Size

    void SetSizeX(float _fNewSizeX);
    void SetSizeY(float _fNewSizeY);
    void SetSize(float _fNewSizeX, float _fNewSizeY);
    void SetSize(const Vector2f& _kNewSize);

    const Vector2f& GetSize() const;       // Get Element Size

    //----------------------------------------------
    // Flip

    void SetFlipV(bool _bFlip);  // Flip Top-Bottom
    void SetFlipH(bool _bFlip);  // Flip Left-Right
    void SetFlip(bool _bFlipV, bool _bFlipH);

    bool GetFlipV() const;
    bool GetFlipH() const;

    //----------------------------------------------
    // Transform

    bool IsPicked(const Vector2f& globalCoords) const;
    bool IsPicked(const Vector2f& globalCoords, Vector2f& localPickedCoords) const;
    virtual bool IsPickedLocal(const Vector2f& localCoords) const;

    Vector2f TransformToLocal(const Vector2f& globalCoords) const;
    Vector2f TransformToLocal(const Vector2f& ancestorCoords, Element* ancestorReference) const;
    Vector2f TransformToGlobal(const Vector2f& localCoords) const;
    Vector2f TransformToGlobal(const Vector2f& localCoords, Element* ancestorReference) const;

    const sf::Transform& GetTransform() const;
    const sf::Transform& GetInverseTransform() const;

    // Get the Element local bounds corners (based on its position and size) projected into global space.
    void GetGlobalCorners(Vector2f& topLeft, Vector2f& topRight, Vector2f& bottomLeft, Vector2f& bottomRight) const;

    // Get the Element global bounds (based on its position and size) as an axis-aligned rectangle.
    sf::FloatRect GetGlobalBounds() const;

    //----------------------------------------------
    // Z-Index

    void SetZIndex(int32 _iZIndex);
    int32 GetZIndex() const;

    void SortOnZIndex();   // recursive

    //----------------------------------------------
    // Visibility

    void SetVisible(bool visible);
    bool IsVisible(bool checkParents = false) const;

    //----------------------------------------------
    // Render

    void RaiseNeedRecompute();

    void Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformParent);

    void SetRenderPass(int _iPass);
    void AddRenderPass(int _iPass);
    void RemoveRenderPass(int _iPass);

    void SetDebugBoundsVisible(bool showDebugBounds);

    //TODO: Handle shaders properly
    // - Need to share shaders between objects ?
    // - Where/how do I manage/load/delete them ?
    // - It should probably only appear in the same places where I use textures.
    // - Maybe it would be a good occasion to add a Material pipeline.
    //void SetShader (sf::Shader* _pShader);

    //----------------------------------------------
    // UI

    void SetUnifiedOrigin(const UDim2& _oNewDimOrigin);
    void SetUnifiedPosition(const UDim2& _oNewDimPos);
    void SetUnifiedSize(const UDim2& _oNewDimSize);
    void SetUnifiedSize(const UDim2& _oNewDimSize, Element* dimSizeReference);

    bool GetUseUnifiedOrigin() const;
    bool GetUseUnifiedPosition() const;
    bool GetUseUnifiedSize() const;
    const UDim2& GetUnifiedOrigin() const;
    const UDim2& GetUnifiedPosition() const;
    const UDim2& GetUnifiedSize() const;

    void ResetUnifiedOrigin();
    void ResetUnifiedPosition();
    void ResetUnifiedSize();

    void ComputeUnifiedOrigin();
    void ComputeUnifiedPosition();
    void ComputeUnifiedSize();
    void ComputeUnifiedDimensionsFromParent();

    //----------------------------------------------
    // Events

    ElementEventHandler* GetEvents();

    //----------------------------------------------
    // Serialization

    bool LoadFromData(ElementDataContext& context);
    bool LoadFromWidgetInstanceData(ElementDataContext& context);

protected:

    //----------------------------------------------
    // Render

    virtual void RecomputeImpl() {}
    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) {}

    //----------------------------------------------
    // Serialization

    bool LoadChildrenFromData(ElementDataContext& context);
    void FillElementPath(ElementDataContext& context);

    virtual bool LoadFromDataImpl(ElementDataContext& context);
    virtual bool LoadFromWidgetInstanceDataImpl(ElementDataContext& context);

    //----------------------------------------------
    // UI

    void OnSizeReferenceDestroyed();
    void OnSizeReferenceChanged();
    void ClearSizeReference();

    //----------------------------------------------
    // Internal Events

    virtual void OnTransformChanged() {}
    virtual void OnVisibleChanged() {}
    virtual void OnSizeChanged() {}

    //----------------------------------------------
    // Z-Index

    static bool CompareZIndex(Element* _pLeft, Element* _pRight);

protected:

    //----------------------------------------------
    // Hierarchy

    Element* m_parent;
    std::vector<Element*> m_children;

    //----------------------------------------------
    // Render

    sf::Transformable m_transform;
    Vector2f m_size;
    bool m_flipV;
    bool m_flipH;

    bool m_isVisible;
    bool m_needRecompute;
    int m_renderPass;
    int32 m_zIndex;

    bool m_showDebugBounds;

    //sf::Shader* m_pShader;

    //----------------------------------------------
    // UI

    bool m_useDimOrigin;
    UDim2 m_dimOrigin;
    bool m_useDimPosition;
    UDim2 m_dimPosition;
    bool m_useDimSize;
    UDim2 m_dimSize;
    Element* m_dimSizeReference;

    //----------------------------------------------
    // Events

    ElementEventHandler* m_eventHandler;
};

}   // namespace gugu
