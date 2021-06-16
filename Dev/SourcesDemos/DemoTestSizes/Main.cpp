////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"

#include "Gugu/Debug/Logger.h"
#include "Gugu/System/SystemUtility.h"

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

    Vector2f m_origin;
    Vector2f m_position;
    float m_rotation;
    Vector2f m_scale;
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
    Vector2f        m_kSize;

    int                 m_iRenderPass;
    int32               m_iZIndex;

    ElementInteractions*    m_pInteractions;

    UDim2       m_oDimOrigin;
    UDim2       m_oDimPos;
    UDim2       m_oDimSize;

    bool        m_bIsVisible;
    bool        m_bFlipV;
    bool        m_bFlipH;
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
    bool b1;
};
class TestBool4
{
    bool b1; bool b2; bool b3; bool b4;
};
class TestBool5
{
    bool b1; bool b2; bool b3; bool b4; bool b5;
};
class TestBool8
{
    bool b1; bool b2; bool b3; bool b4; bool b5; bool b6; bool b7; bool b8;
};

class TestMarginBool1
{
    int margin;
    bool b1;
};
class TestMarginBool4
{
    int margin;
    bool b1; bool b2; bool b3; bool b4;
};
class TestMarginBool5
{
    int margin;
    bool b1; bool b2; bool b3; bool b4; bool b5;
};
class TestMarginBool8
{
    int margin;
    bool b1; bool b2; bool b3; bool b4; bool b5; bool b6; bool b7; bool b8;
};

int main(int argc, char* argv[])
{
#if defined(GUGU_ENV_VISUAL )

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    //----------------------------------------------
    // Here is some tests for the standard tree functions on the Element class.

    // Create a new Element A.
    Element* elementA = new Element;

    // Create a new Element B, add it as a child to A.
    Element* elementB = new Element;
    elementA->AddChild(elementB);

    // Create a new Element C directly as a child to A.
    Element* elementC = elementA->AddChild<Element>();

    // Remove then Add B to A, it should now be placed after C in the list of A's children.
    elementA->RemoveChild(elementB);
    elementA->AddChild(elementB);

    // Add C as a child to B, it should be properly removed from A's children.
    elementB->AddChild(elementC);

    //----------------------------------------------
    // Here is some tests on various Element class sizes, and attempts to reduce those that could be applied in the future.

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
    log.Print(ELog::Info, StringFormat(" TestMarginBool1 : {0}", sizeof(TestMarginBool1)));
    log.Print(ELog::Info, StringFormat(" TestMarginBool4 : {0}", sizeof(TestMarginBool4)));
    log.Print(ELog::Info, StringFormat(" TestMarginBool5 : {0}", sizeof(TestMarginBool5)));
    log.Print(ELog::Info, StringFormat(" TestMarginBool8 : {0}", sizeof(TestMarginBool8)));
    log.Print(ELog::Info, StringFormat("---------------------------------------"));
    log.Print(ELog::Info, StringFormat(" Vector2f : {0}", sizeof(Vector2f)));
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
    log.Print(ELog::Info, StringFormat(" ElementSpriteAnimated : {0}", sizeof(ElementSpriteAnimated)));
    log.Print(ELog::Info, StringFormat(" ElementInteractions : {0}", sizeof(ElementInteractions)));
    log.Print(ELog::Info, StringFormat("---------------------------------------"));
    log.Print(ELog::Info, StringFormat(" TestSFTransformable : {0}", sizeof(TestSFTransformable)));
    log.Print(ELog::Info, StringFormat(" TestElement : {0}", sizeof(TestElement)));
    log.Print(ELog::Info, StringFormat(" TestElementSprite : {0}", sizeof(TestElementSprite)));
    log.Print(ELog::Info, StringFormat("---------------------------------------"));

    //----------------------------------------------

    return 0;
}
