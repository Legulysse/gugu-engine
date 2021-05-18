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

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementSpriteGroupItem::ElementSpriteGroupItem()
: m_tiled(false)
{
}

ElementSpriteGroupItem::~ElementSpriteGroupItem()
{
}

void ElementSpriteGroupItem::SetSubRect(const sf::IntRect& _kSubRect)
{
    m_subRect = _kSubRect;
}

void ElementSpriteGroupItem::SetTiled(bool tiled)
{
    m_tiled = tiled;
}

int ElementSpriteGroupItem::GetItemVertexCount() const
{
    return 6;
}

bool ElementSpriteGroupItem::ComputeItemVertices(sf::VertexArray& vertices, int& indexFirstVertex) const
{
    sf::Vertex* quad = &vertices[indexFirstVertex];
    const sf::Transform& kTransform = GetTransform();

    // Position
    float fSurfaceLeft = 0.f;
    float fSurfaceTop = 0.f;
    float fSurfaceRight = GetSize().x;
    float fSurfaceBottom = GetSize().y;

    quad[0].position = kTransform * sf::Vector2f(fSurfaceLeft, fSurfaceTop);
    quad[1].position = kTransform * sf::Vector2f(fSurfaceRight, fSurfaceTop);
    quad[2].position = kTransform * sf::Vector2f(fSurfaceLeft, fSurfaceBottom);

    quad[3].position = kTransform * sf::Vector2f(fSurfaceRight, fSurfaceTop);
    quad[4].position = kTransform * sf::Vector2f(fSurfaceLeft, fSurfaceBottom);
    quad[5].position = kTransform * sf::Vector2f(fSurfaceRight, fSurfaceBottom);

    // Texture
    float fTextureLeft = (float)m_subRect.left;
    float fTextureTop = (float)m_subRect.top;
    float fTextureRight = fTextureLeft + m_subRect.width;
    float fTextureBottom = fTextureTop + m_subRect.height;

    quad[0].texCoords = sf::Vector2f(fTextureLeft, fTextureTop);
    quad[1].texCoords = sf::Vector2f(fTextureRight, fTextureTop);
    quad[2].texCoords = sf::Vector2f(fTextureLeft, fTextureBottom);

    quad[3].texCoords = sf::Vector2f(fTextureRight, fTextureTop);
    quad[4].texCoords = sf::Vector2f(fTextureLeft, fTextureBottom);
    quad[5].texCoords = sf::Vector2f(fTextureRight, fTextureBottom);

    // Update the current group index for the next item
    indexFirstVertex += 6;
    return true;
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
        m_needRecompute = true;
    }
}

Texture* ElementSpriteGroup::GetTexture() const
{
    return m_texture;
}

void ElementSpriteGroup::SetSizeImpl(sf::Vector2f _kOldSize)
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        m_items[i]->OnParentResized(_kOldSize, GetSize());
    }

    m_needRecompute = true;
}

void ElementSpriteGroup::DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
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

            int nbVertices = 0;
            for (size_t i = 0; i < m_items.size(); ++i)
            {
                if (m_items[i]->IsVisible(false))
                {
                    nbVertices += m_items[i]->GetItemVertexCount();
                }
            }

            // Reset vertices
            m_vertices.setPrimitiveType(sf::Triangles);
            m_vertices.resize(nbVertices);

            if (nbVertices > 0)
            {
                int indexFirstVertex = 0;
                for (size_t i = 0; i < m_items.size(); ++i)
                {
                    if (m_items[i]->IsVisible(false))
                    {
                        m_items[i]->ComputeItemVertices(m_vertices, indexFirstVertex);
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
                _kRenderPass.frameInfos->statDrawCalls += 1;

            //TODO: special stat category for ElementSpriteGroup
        }
    }
}

void ElementSpriteGroup::RecomputeItemVertices(int _iIndex)
{
    // TODO: if called explicitly, maybe I should allow it ? or add a "force" parameter ?
    if (m_needRecompute)
        return;

    int indexFirstVertex = 0;
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if ((int)i == _iIndex)
        {
            m_items[i]->ComputeItemVertices(m_vertices, indexFirstVertex);
            break;
        }
        else
        {
            if (m_items[i]->IsVisible(false))
            {
                indexFirstVertex += m_items[i]->GetItemVertexCount();
            }
        }
    }
}

int ElementSpriteGroup::AddItem(ElementSpriteGroupItem* _pNewItem)
{
    _pNewItem->SetParent(this, false);
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

            pNewTile->SetTiled(true);
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
    //SetSizeImpl(GetSize());
}

}   // namespace gugu
