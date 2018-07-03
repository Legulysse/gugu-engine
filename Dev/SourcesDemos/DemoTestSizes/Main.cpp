////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Element/2D/ElementTile.h"

#include "Gugu/Misc/Logger.h"
#include "Gugu/Utility/System.h"

#include <SFML/Graphics/Sprite.hpp>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

class TestSFTransformable
{
    // Removed virtual
    // Grouped boolean members

public:

    ~TestSFTransformable() {}

    sf::Vector2f m_origin;
    sf::Vector2f m_position;
    float m_rotation;
    sf::Vector2f m_scale;
    mutable sf::Transform m_transform;
    mutable sf::Transform m_inverseTransform;
    mutable bool m_inverseTransformNeedUpdate;
    mutable bool m_transformNeedUpdate;
};

class TestElement
{
    // Grouped boolean members
    // Used TestTransformable

public:

    virtual ~TestElement() {}

    Element*            m_pParent;
    std::vector<Element*> m_listChildren;

    TestSFTransformable m_kTransform;
    sf::Vector2f        m_kSize;

    int                 m_iRenderPass;
    int32               m_iZIndex;

    ElementInteractions*    m_pInteractions;

    UDim2       m_oDimOrigin;
    UDim2       m_oDimPos;
    UDim2       m_oDimSize;

    bool        m_bIsVisible;
    bool        m_bFlipX;
    bool        m_bFlipY;
    bool        m_bDimOrigin;
    bool        m_bDimPos;
    bool        m_bDimSize;
};

class TestElementSprite : public TestElement
{
    // Removed m_kColor

public:

    virtual ~TestElementSprite() {}

    Texture* m_pTexture;
    sf::IntRect m_kSubRect;

    sf::VertexArray m_kVertices;

    bool m_bFlipTextureX;
    bool m_bFlipTextureY;
};

class TestBool1
{
    int margin;

    bool b1;
};
class TestBool4
{
    int margin;

    bool b1; bool b2; bool b3; bool b4;
};
class TestBool5
{
    int margin;

    bool b1; bool b2; bool b3; bool b4; bool b5;
};
class TestBool8
{
    int margin;

    bool b1; bool b2; bool b3; bool b4; bool b5; bool b6; bool b7; bool b8;
};


int main(int argc, char* argv[])
{
#if defined(GUGU_ENV_VISUAL )

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    //--------

    Logger log;
    log.SetUseTimestamp(false);
    //log.SetFile("TestSizes.log");

    log.Print(ELog::Info, StringFormat("---------------------------------------"));
    log.Print(ELog::Info, StringFormat(" bool : {0}", sizeof(bool)));
    log.Print(ELog::Info, StringFormat(" int : {0}", sizeof(int)));
    log.Print(ELog::Info, StringFormat(" float : {0}", sizeof(float)));
    log.Print(ELog::Info, StringFormat(" float[16] : {0} (= sf::Transform)", sizeof(float[16])));
    log.Print(ELog::Info, StringFormat("---------------------------------------"));
    log.Print(ELog::Info, StringFormat(" TestBool1 : {0}", sizeof(TestBool1)));
    log.Print(ELog::Info, StringFormat(" TestBool4 : {0}", sizeof(TestBool4)));
    log.Print(ELog::Info, StringFormat(" TestBool5 : {0}", sizeof(TestBool5)));
    log.Print(ELog::Info, StringFormat(" TestBool8 : {0}", sizeof(TestBool8)));
    log.Print(ELog::Info, StringFormat("---------------------------------------"));
    log.Print(ELog::Info, StringFormat(" sf::Vector2f : {0}", sizeof(sf::Vector2f)));
    log.Print(ELog::Info, StringFormat(" sf::Transformable : {0}", sizeof(sf::Transformable)));
    log.Print(ELog::Info, StringFormat(" sf::Transform : {0}", sizeof(sf::Transform)));
    log.Print(ELog::Info, StringFormat(" sf::Sprite : {0}", sizeof(sf::Sprite)));
    log.Print(ELog::Info, StringFormat(" sf::Color : {0}", sizeof(sf::Color)));
    log.Print(ELog::Info, StringFormat(" sf::IntRect : {0}", sizeof(sf::IntRect)));
    log.Print(ELog::Info, StringFormat(" std::vector<Element*> : {0}", sizeof(std::vector<Element*>)));
    log.Print(ELog::Info, StringFormat("---------------------------------------"));
    log.Print(ELog::Info, StringFormat(" UDim : {0}", sizeof(UDim)));
    log.Print(ELog::Info, StringFormat(" UDim2 : {0}", sizeof(UDim2)));
    log.Print(ELog::Info, StringFormat(" Element : {0}", sizeof(Element)));
    log.Print(ELog::Info, StringFormat(" ElementSprite : {0}", sizeof(ElementSprite)));
    log.Print(ELog::Info, StringFormat(" ElementTile : {0}", sizeof(ElementTile)));
    log.Print(ELog::Info, StringFormat(" ElementSpriteAnimated : {0}", sizeof(ElementSpriteAnimated)));
    log.Print(ELog::Info, StringFormat(" ElementInteractions : {0}", sizeof(ElementInteractions)));
    log.Print(ELog::Info, StringFormat("---------------------------------------"));
    log.Print(ELog::Info, StringFormat(" TestSFTransformable : {0}", sizeof(TestSFTransformable)));
    log.Print(ELog::Info, StringFormat(" TestElement : {0}", sizeof(TestElement)));
    log.Print(ELog::Info, StringFormat(" TestElementSprite : {0}", sizeof(TestElementSprite)));
    log.Print(ELog::Info, StringFormat("---------------------------------------"));

    //--------

    return 0;
}
