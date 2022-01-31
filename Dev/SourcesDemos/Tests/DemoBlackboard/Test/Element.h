////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Element/2D/ElementSprite.h"

#include <SFML/System/Clock.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace test {

//----------------------------------------------

void RunTestElement()
{
    using namespace gugu;

    //----------------------------------------------

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

    // Delete A, it should delete all its hierarchy and invalidate B and C pointers.
    SafeDelete(elementA);
    elementB = nullptr;
    elementC = nullptr;

    //----------------------------------------------

#if 0

    Element* root = new Element;
    root->SetSize(100.f, 100.f);

    {
        sf::Clock kClock;

        size_t nbChildren = 100;
        size_t nbGrandChildren = 100;
        for (size_t i = 0; i < nbChildren; ++i)
        {
            Element* child = root->AddChild<Element>();
            child->SetUnifiedOrigin(UDim2::POSITION_CENTER);
            child->SetUnifiedPosition(UDim2::POSITION_CENTER);
            child->SetUnifiedSize(UDim2::SIZE_SQUARE_QUARTER);

            for (size_t ii = 0; ii < nbGrandChildren; ++ii)
            {
                Element* grandChild = child->AddChild<Element>();
                grandChild->SetUnifiedOrigin(UDim2::POSITION_CENTER);
                grandChild->SetUnifiedPosition(UDim2::POSITION_CENTER);
                grandChild->SetUnifiedSize(UDim2::SIZE_SQUARE_QUARTER);
            }
        }

        sf::Time result = kClock.getElapsedTime();
        sf::Int32 ms = result.asMilliseconds();

        WriteInConsoleEndline(StringFormat("Create hierarchy : {0} ms", ms), true);
    }

    {
        sf::Clock kClock;
        std::string strFormat = "";

        size_t nbLoops = 500;
        for (size_t i = 0; i < nbLoops; ++i)
        {
            root->SetSize(i % 2 == 0 ? Vector2f(50.f, 50.f) : Vector2f(150.f, 150.f));
        }

        sf::Time result = kClock.getElapsedTime();
        sf::Int32 ms = result.asMilliseconds();

        WriteInConsoleEndline(StringFormat("Update size : {0} ms", ms), true);
    }

    {
        sf::Clock kClock;
        std::string strFormat = "";

        SafeDelete(root);

        sf::Time result = kClock.getElapsedTime();
        sf::Int32 ms = result.asMilliseconds();

        WriteInConsoleEndline(StringFormat("Delete hierarchy : {0} ms", ms), true);
    }
#endif
}

}   //namespace test
