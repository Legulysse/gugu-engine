////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Events/ElementEventHandler.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Graphics/Sprite.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace test {

//----------------------------------------------

class TestSFTransformable
{
    // Removed virtual
    // Grouped boolean members

public:

    ~TestSFTransformable() {}

    gugu::Vector2f m_origin;
    gugu::Vector2f m_position;
    float m_rotation;
    gugu::Vector2f m_scale;
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

    gugu::Element* m_pParent;
    std::vector<gugu::Element*> m_listChildren;

    TestSFTransformable m_kTransform;
    gugu::Vector2f        m_kSize;

    int                 m_iRenderPass;
    gugu::int32               m_iZIndex;

    gugu::ElementEventHandler* m_pInteractions;

    gugu::UDim2       m_oDimOrigin;
    gugu::UDim2       m_oDimPos;
    gugu::UDim2       m_oDimSize;

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

    gugu::Texture* m_pTexture;
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

//----------------------------------------------

void RunTestObjectSizes()
{
    using namespace gugu;

    //----------------------------------------------

#if 0

    Logger log;
    log.SetConsoleOutput(true, true);
    //log.SetFile("TestSizes.log");

    log.Print(StringFormat("---------------------------------------"));
    log.Print(StringFormat(" bool : {0}", sizeof(bool)));
    log.Print(StringFormat(" int : {0}", sizeof(int)));
    log.Print(StringFormat(" float : {0}", sizeof(float)));
    log.Print(StringFormat(" float[16] : {0} (= sf::Transform)", sizeof(float[16])));
    log.Print(StringFormat("---------------------------------------"));
    log.Print(StringFormat(" TestBool1 : {0}", sizeof(TestBool1)));
    log.Print(StringFormat(" TestBool4 : {0}", sizeof(TestBool4)));
    log.Print(StringFormat(" TestBool5 : {0}", sizeof(TestBool5)));
    log.Print(StringFormat(" TestBool8 : {0}", sizeof(TestBool8)));
    log.Print(StringFormat("---------------------------------------"));
    log.Print(StringFormat(" TestMarginBool1 : {0}", sizeof(TestMarginBool1)));
    log.Print(StringFormat(" TestMarginBool4 : {0}", sizeof(TestMarginBool4)));
    log.Print(StringFormat(" TestMarginBool5 : {0}", sizeof(TestMarginBool5)));
    log.Print(StringFormat(" TestMarginBool8 : {0}", sizeof(TestMarginBool8)));
    log.Print(StringFormat("---------------------------------------"));
    log.Print(StringFormat(" Vector2f : {0}", sizeof(Vector2f)));
    log.Print(StringFormat(" sf::Transformable : {0}", sizeof(sf::Transformable)));
    log.Print(StringFormat(" sf::Transform : {0}", sizeof(sf::Transform)));
    log.Print(StringFormat(" sf::Sprite : {0}", sizeof(sf::Sprite)));
    log.Print(StringFormat(" sf::Color : {0}", sizeof(sf::Color)));
    log.Print(StringFormat(" sf::IntRect : {0}", sizeof(sf::IntRect)));
    log.Print(StringFormat(" std::vector<Element*> : {0}", sizeof(std::vector<Element*>)));
    log.Print(StringFormat("---------------------------------------"));
    log.Print(StringFormat(" UDim : {0}", sizeof(UDim)));
    log.Print(StringFormat(" UDim2 : {0}", sizeof(UDim2)));
    log.Print(StringFormat(" Element : {0}", sizeof(Element)));
    log.Print(StringFormat(" ElementSprite : {0}", sizeof(ElementSprite)));
    log.Print(StringFormat(" ElementEventHandler : {0}", sizeof(ElementEventHandler)));
    log.Print(StringFormat("---------------------------------------"));
    log.Print(StringFormat(" TestSFTransformable : {0}", sizeof(TestSFTransformable)));
    log.Print(StringFormat(" TestElement : {0}", sizeof(TestElement)));
    log.Print(StringFormat(" TestElementSprite : {0}", sizeof(TestElementSprite)));
    log.Print(StringFormat("---------------------------------------"));

#endif
}

}   //namespace test
