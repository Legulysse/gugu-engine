////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/External/PugiXmlWrap.h"

#include "Gugu/Engine.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementSpriteGroupItem::ElementSpriteGroupItem()
    : m_spriteGroup(nullptr)
    , m_cachedVertexCount(0)
{
}

ElementSpriteGroupItem::~ElementSpriteGroupItem()
{
}

void ElementSpriteGroupItem::SetSpriteGroup(ElementSpriteGroup* spriteGroup)
{
    m_spriteGroup = spriteGroup;

    if (!m_parent)
    {
        SetParent(m_spriteGroup);
    }
}

void ElementSpriteGroupItem::RaiseDirtyVertices()
{
    ElementSpriteBase::RaiseDirtyVertices();

    if (m_spriteGroup)
    {
        m_spriteGroup->RaiseNeedRecompute();
    }
}

void ElementSpriteGroupItem::OnTransformChanged()
{
    RaiseDirtyVertices();
}

void ElementSpriteGroupItem::OnVisibleChanged()
{
    RaiseDirtyVertices();
}

bool ElementSpriteGroupItem::HasDirtyVertices() const
{
    return m_dirtyVertices;
}

size_t ElementSpriteGroupItem::RecomputeVertexCount()
{
    m_cachedVertexCount = ElementSpriteBase::GetRequiredVertexCount();
    return m_cachedVertexCount;
}

size_t ElementSpriteGroupItem::GetCachedVertexCount() const
{
    return m_cachedVertexCount;
}

size_t ElementSpriteGroupItem::RecomputeItemVertices(sf::VertexArray& vertices, size_t indexFirstVertex)
{
    m_dirtyVertices = false;

    ElementSpriteBase::RecomputeVerticesPositionAndTextureCoords(&vertices[indexFirstVertex]);

    // Combine generated vertices with the item transform.
    const sf::Transform& transform = GetTransform();
    for (size_t i = 0; i < m_cachedVertexCount; ++i)
    {
        vertices[indexFirstVertex + i].position = transform * vertices[indexFirstVertex + i].position;
    }

    return m_cachedVertexCount;
}


ElementSpriteGroup::ElementSpriteGroup()
: m_texture(nullptr)
, m_needRecompute(true)
{
}

ElementSpriteGroup::~ElementSpriteGroup()
{
    ClearStdVector(m_items);
}

void ElementSpriteGroup::SetTexture(const std::string& _strTexturePath)
{
    SetTexture(GetResources()->GetTexture(_strTexturePath));
}

void ElementSpriteGroup::SetTexture(Texture* _pTexture)
{
    if (_pTexture)
    {
        m_texture = _pTexture;
    }
}

Texture* ElementSpriteGroup::GetTexture() const
{
    return m_texture;
}

void ElementSpriteGroup::OnSizeChanged(sf::Vector2f _kOldSize)
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        m_items[i]->OnParentResized(_kOldSize, GetSize());
    }
}

void ElementSpriteGroup::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_texture || !m_texture->GetSFTexture())
        return;

    //TODO: maybe need a parameter to bypass this check ?
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(sf::Vector2f(), m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        if (m_needRecompute)
        {
            m_needRecompute = false;

            size_t indexForceRecompute = (size_t)-1;

            size_t totalVertexCount = 0;
            for (size_t i = 0; i < m_items.size(); ++i)
            {
                size_t cachedVertexCount = m_items[i]->GetCachedVertexCount();
                
                size_t currentVertexCount = 0;
                if (m_items[i]->IsVisible())
                {
                    currentVertexCount = m_items[i]->RecomputeVertexCount();
                    totalVertexCount += currentVertexCount;
                }

                if (indexForceRecompute == (size_t)-1 && cachedVertexCount != currentVertexCount)
                {
                    indexForceRecompute = i;
                }
            }

            // Reset vertices
            m_vertices.setPrimitiveType(sf::Triangles);
            m_vertices.resize(totalVertexCount);

            if (totalVertexCount > 0)
            {
                size_t indexItemVertices = 0;
                for (size_t i = 0; i < m_items.size(); ++i)
                {
                    if (m_items[i]->IsVisible())
                    {
                        if (i >= indexForceRecompute || m_items[i]->HasDirtyVertices())
                        {
                            indexItemVertices += m_items[i]->RecomputeItemVertices(m_vertices, indexItemVertices);
                        }
                        else
                        {
                            indexItemVertices += m_items[i]->GetCachedVertexCount();
                        }
                    }
                }
            }
        }

        if (m_vertices.getVertexCount() > 0)
        {
            sf::RenderStates states;
            states.transform = _kTransformSelf;
            states.texture = m_texture->GetSFTexture();
            _kRenderPass.target->draw(m_vertices, states);

            //Stats
            if (_kRenderPass.frameInfos)
            {
                _kRenderPass.frameInfos->statDrawCalls += 1;
                _kRenderPass.frameInfos->statTriangles += m_vertices.getVertexCount() / 3;
            }

            //TODO: special stat category for ElementSpriteGroup
        }
    }
}

void ElementSpriteGroup::RaiseNeedRecompute()
{
    m_needRecompute = true;
}

int ElementSpriteGroup::AddItem(ElementSpriteGroupItem* _pNewItem)
{
    _pNewItem->SetSpriteGroup(this);
    m_items.push_back(_pNewItem);

    m_needRecompute = true;
    return m_items.size() - 1;
}

ElementSpriteGroupItem* ElementSpriteGroup::GetItem(int _iIndex) const
{
    if (_iIndex < 0 || _iIndex >= (int)m_items.size())
        return nullptr;
    return m_items[_iIndex];
}

void ElementSpriteGroup::GetPropagationList(std::vector<Element*>& _vecPropagationList)
{
    StdVectorAppend(m_items, _vecPropagationList);
}

void ElementSpriteGroup::LoadFromXml(const std::string& _strPath)
{
    pugi::xml_document kDoc;
    pugi::xml_parse_result kResult = kDoc.load_file(GetResources()->GetResourcePathName(_strPath).c_str());
    if (!kResult)
        return;

    pugi::xml_node kNodeRoot = kDoc.child("ElementSpriteGroup");
    if (!kNodeRoot)
        return;

    // Either use an ImageSet + SubImages, or a Texture + Rects
    ImageSet* pImageSet = nullptr;
    Texture* pTexture = nullptr;

    pugi::xml_attribute kAttrImageSet = kNodeRoot.attribute("ImageSet");
    if (kAttrImageSet)
    {
        pImageSet = GetResources()->GetImageSet(kAttrImageSet.as_string());
    }

    if (!pImageSet)
    {
        pugi::xml_attribute kAttrTexture = kNodeRoot.attribute("Texture");
        if (kAttrTexture)
        {
            pTexture = GetResources()->GetTexture(kAttrTexture.as_string());
        }
        
        if (!pTexture)
        {
            return;
        }
    }

    if (pImageSet)
        SetTexture(pImageSet->GetTexture());
    else if (pTexture)
        SetTexture(pTexture);

    for (pugi::xml_node kNodeComponent = kNodeRoot.child("Component"); kNodeComponent; kNodeComponent = kNodeComponent.next_sibling("Component"))
    {
        pugi::xml_attribute kAttrType = kNodeComponent.attribute("Type");
        if (!kAttrType)
            continue;

        std::string strType = kAttrType.as_string();
        ElementSpriteGroupItem* pNewElement = nullptr;

        if (strType == "Sprite")
        {
            ElementSpriteGroupItem* pNewSprite = new ElementSpriteGroupItem;
            pNewElement = pNewSprite;
        }
        else if (strType == "Tile")
        {
            ElementSpriteGroupItem* pNewTile = new ElementSpriteGroupItem;
            pNewElement = pNewTile;

            pNewTile->SetRepeatTexture(true);
        }

        if (pNewElement)
        {
            // Texture
            if (pImageSet)
            {
                pugi::xml_attribute kAttrSubImage = kNodeComponent.attribute("SubImage");
                if (kAttrSubImage)
                {
                    SubImage* pSubImage = pImageSet->GetSubImage(kAttrSubImage.as_string());
                    if (pSubImage)
                        pNewElement->SetSubRect(pSubImage->GetRect());
                }
            }
            else if (pTexture)
            {
                pugi::xml_node kNodeRect = kNodeComponent.child("TextureRect");
                if (kNodeRect)
                {
                    pugi::xml_attribute kAttrX = kNodeRect.attribute("X");
                    pugi::xml_attribute kAttrY = kNodeRect.attribute("Y");
                    pugi::xml_attribute kAttrW = kNodeRect.attribute("W");
                    pugi::xml_attribute kAttrH = kNodeRect.attribute("H");

                    if (kAttrX
                        && kAttrY
                        && kAttrW
                        && kAttrH)
                    {
                        int x = kAttrX.as_int();
                        int y = kAttrY.as_int();
                        int w = kAttrW.as_int();
                        int h = kAttrH.as_int();

                        pNewElement->SetSubRect(sf::IntRect(x, y, w, h));
                    }
                }
            }

            // Position and Size
            pugi::xml_node kNodePosition = kNodeComponent.child("Position");
            pugi::xml_node kNodeSize = kNodeComponent.child("Size");

            UDim2 kDim;

            if (kNodePosition && XmlReadUDimension(kNodePosition, kDim))
                pNewElement->SetUnifiedPosition(kDim);

            if (kNodeSize && XmlReadUDimension(kNodeSize, kDim))
                pNewElement->SetUnifiedSize(kDim);

            // Finalize
            AddItem(pNewElement);
        }
    }

    m_needRecompute = true;
}

}   // namespace gugu
