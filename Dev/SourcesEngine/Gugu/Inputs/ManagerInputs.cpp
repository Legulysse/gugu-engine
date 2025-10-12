////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Inputs/ManagerInputs.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "gugu/Window/Window.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerInputs::ManagerInputs()
{
}

ManagerInputs::~ManagerInputs()
{
}

void ManagerInputs::Init(const EngineConfig& config)
{
    FillListKeyCodes();
}

void ManagerInputs::Release()
{
    m_inputBindings.clear();
    m_keyCodes.clear();
}

void ManagerInputs::RegisterInput(const std::string& _strInputName, const sf::Event& _oSFEvent)
{
    m_inputBindings[_strInputName].bindings.push_back(_oSFEvent);
}

bool ManagerInputs::ModifyInput(const std::string& _strInputName, const sf::Event& _oSFEvent, uint32 _uiIndex)
{
    auto kvp = m_inputBindings.find(_strInputName);
    if (kvp != m_inputBindings.end())
    {
        std::vector<sf::Event>& vecBindings = kvp->second.bindings;
        if (_uiIndex < vecBindings.size())
        {
            vecBindings[_uiIndex] = _oSFEvent;
        }
    }

    return false;
}

void ManagerInputs::LoadInputFile(const std::string& _strPath)
{
    pugi::xml_document oDoc;
    pugi::xml_parse_result result = oDoc.load_file(GetResources()->GetResourceFileInfo(_strPath).GetFileSystemPath().c_str());
    if (!result)
        return;

    pugi::xml_node oNodeBindings = oDoc.child("Bindings");
    if (!oNodeBindings)
        return;
    
    std::string strInputName, strKey;
    sf::Keyboard::Key oKeyCode = sf::Keyboard::Key::Unknown;

    for (pugi::xml_node oNodeInput = oNodeBindings.child("Input"); oNodeInput; oNodeInput = oNodeInput.next_sibling("Input"))
    {
        pugi::xml_attribute oAttributeName = oNodeInput.attribute("name");
        if (!oAttributeName)
            continue;

        // TODO: Joystick and Mouse buttons.
        for (pugi::xml_node oNodeKey = oNodeInput.child("Key"); oNodeKey; oNodeKey = oNodeKey.next_sibling("Key"))
        {
            pugi::xml_attribute oAttributeValue = oNodeKey.attribute("value");

            if (oAttributeValue && ReadKeyCode(oAttributeValue.as_string(), oKeyCode))
                RegisterInput(oAttributeName.as_string(), BuildKeyboardEvent(oKeyCode));
        }
    }
}

bool ManagerInputs::IsInputEvent(const std::string& _strInputName, const sf::Event& _oSFEvent) const
{
    if (!IsInputAllowed())
    {
        return false;
    }

    auto kvp = m_inputBindings.find(_strInputName);
    if (kvp != m_inputBindings.end())
    {
        const std::vector<sf::Event>& vecBindings = kvp->second.bindings;
        for (size_t i = 0; i < vecBindings.size(); ++i)
        {
            const sf::Event& oStoredEvent = vecBindings[i];

            // Keyboard
            if (oStoredEvent.type == sf::Event::KeyPressed)
            {
                if (_oSFEvent.type == sf::Event::KeyPressed || _oSFEvent.type == sf::Event::KeyReleased)
                {
                    if (oStoredEvent.key.code == _oSFEvent.key.code
                        && (oStoredEvent.key.control == IsControlDown())
                        && (oStoredEvent.key.shift == IsShiftDown())
                        && (oStoredEvent.key.alt == IsAltDown()))
                        return true;
                }
            }
            // Mouse
            else if (oStoredEvent.type == sf::Event::MouseButtonPressed)
            {
                if (_oSFEvent.type == sf::Event::MouseButtonPressed || _oSFEvent.type == sf::Event::MouseButtonReleased)
                {
                    if (oStoredEvent.mouseButton.button == _oSFEvent.mouseButton.button)
                        return true;
                }
            }
            // Joystick
            else if (oStoredEvent.type == sf::Event::JoystickButtonPressed)
            {
                if (_oSFEvent.type == sf::Event::JoystickButtonPressed || _oSFEvent.type == sf::Event::JoystickButtonReleased)
                {
                    if (oStoredEvent.joystickButton.joystickId == _oSFEvent.joystickButton.joystickId && oStoredEvent.joystickButton.button == _oSFEvent.joystickButton.button)
                        return true;
                }
            }
        }
    }

    return false;
}

bool ManagerInputs::IsInputDown(const std::string& _strInputName) const
{
    if (!IsInputAllowed())
    {
        return false;
    }

    auto kvp = m_inputBindings.find(_strInputName);
    if (kvp != m_inputBindings.end())
    {
        const std::vector<sf::Event>& vecBindings = kvp->second.bindings;
        for (size_t i = 0; i < vecBindings.size(); ++i)
        {
            const sf::Event& oStoredEvent = vecBindings[i];

            // Keyboard
            if (oStoredEvent.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(oStoredEvent.key.code)
                    && (oStoredEvent.key.control == IsControlDown())
                    && (oStoredEvent.key.shift == IsShiftDown())
                    && (oStoredEvent.key.alt == IsAltDown()))
                    return true;
            }
            // Mouse
            else if (oStoredEvent.type == sf::Event::MouseButtonPressed)
            {
                if (sf::Mouse::isButtonPressed(oStoredEvent.mouseButton.button))
                    return true;
            }
            // Joystick
            else if (oStoredEvent.type == sf::Event::JoystickButtonPressed)
            {
                if (sf::Joystick::isButtonPressed(oStoredEvent.joystickButton.joystickId, oStoredEvent.joystickButton.button))
                    return true;
            }
        }
    }

    return false;
}

bool ManagerInputs::IsInputEventPressed(const std::string& _strInputName, const sf::Event& _oSFEvent) const
{
    return IsInputAllowed()
        && (_oSFEvent.type == sf::Event::KeyPressed || _oSFEvent.type == sf::Event::MouseButtonPressed || _oSFEvent.type == sf::Event::JoystickButtonPressed)
        && IsInputEvent(_strInputName, _oSFEvent);
}

bool ManagerInputs::IsInputEventReleased(const std::string& _strInputName, const sf::Event& _oSFEvent) const
{
    return IsInputAllowed()
        && (_oSFEvent.type == sf::Event::KeyReleased || _oSFEvent.type == sf::Event::MouseButtonPressed || _oSFEvent.type == sf::Event::JoystickButtonReleased)
        && IsInputEvent(_strInputName, _oSFEvent);
}

bool ManagerInputs::IsControlDown() const
{
    return IsInputAllowed()
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl));
}

bool ManagerInputs::IsShiftDown() const
{
    return IsInputAllowed()
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift));
}

bool ManagerInputs::IsAltDown() const
{
    return IsInputAllowed()
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt));
}

bool ManagerInputs::IsKeyDown(sf::Keyboard::Key _eKey) const
{
    return IsInputAllowed()
        && sf::Keyboard::isKeyPressed(_eKey);
}

bool ManagerInputs::IsButtonDown(sf::Mouse::Button button) const
{
    return IsInputAllowed()
        && sf::Mouse::isButtonPressed(button);
}

sf::Event ManagerInputs::BuildKeyboardEvent(sf::Keyboard::Key key)
{
    return ManagerInputs::BuildKeyboardEvent(key, false, false, false);
}

sf::Event ManagerInputs::BuildKeyboardEvent(sf::Keyboard::Key key, bool control, bool shift, bool alt)
{
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = key;
    event.key.alt = alt;
    event.key.shift = shift;
    event.key.control = control;
    event.key.system = false;
    return event;
}

sf::Event ManagerInputs::BuildMouseEvent(sf::Mouse::Button button)
{
    sf::Event event;
    event.type = sf::Event::MouseButtonPressed;
    event.mouseButton.button = button;
    return event;
}

sf::Event ManagerInputs::BuildJoystickEvent(EPadButton button, int joystickId)
{
    return ManagerInputs::BuildJoystickEvent((int)button, joystickId);
}

sf::Event ManagerInputs::BuildJoystickEvent(int button, int joystickId)
{
    sf::Event event;
    event.type = sf::Event::JoystickButtonPressed;
    event.joystickButton.joystickId = joystickId;
    event.joystickButton.button = button;
    return event;
}

void ManagerInputs::FillListKeyCodes()
{
    m_keyCodes["A"] = sf::Keyboard::Key::A;
    m_keyCodes["B"] = sf::Keyboard::Key::B;
    m_keyCodes["C"] = sf::Keyboard::Key::C;
    m_keyCodes["D"] = sf::Keyboard::Key::D;
    m_keyCodes["E"] = sf::Keyboard::Key::E;
    m_keyCodes["F"] = sf::Keyboard::Key::F;
    m_keyCodes["G"] = sf::Keyboard::Key::G;
    m_keyCodes["H"] = sf::Keyboard::Key::H;
    m_keyCodes["I"] = sf::Keyboard::Key::I;
    m_keyCodes["J"] = sf::Keyboard::Key::J;
    m_keyCodes["K"] = sf::Keyboard::Key::K;
    m_keyCodes["L"] = sf::Keyboard::Key::L;
    m_keyCodes["M"] = sf::Keyboard::Key::M;
    m_keyCodes["N"] = sf::Keyboard::Key::N;
    m_keyCodes["O"] = sf::Keyboard::Key::O;
    m_keyCodes["P"] = sf::Keyboard::Key::P;
    m_keyCodes["Q"] = sf::Keyboard::Key::Q;
    m_keyCodes["R"] = sf::Keyboard::Key::R;
    m_keyCodes["S"] = sf::Keyboard::Key::S;
    m_keyCodes["T"] = sf::Keyboard::Key::T;
    m_keyCodes["U"] = sf::Keyboard::Key::U;
    m_keyCodes["V"] = sf::Keyboard::Key::V;
    m_keyCodes["W"] = sf::Keyboard::Key::W;
    m_keyCodes["X"] = sf::Keyboard::Key::X;
    m_keyCodes["Y"] = sf::Keyboard::Key::Y;
    m_keyCodes["Z"] = sf::Keyboard::Key::Z;

    m_keyCodes["Num0"] = sf::Keyboard::Key::Num0;     ///< The 0 key
    m_keyCodes["Num1"] = sf::Keyboard::Key::Num1;     ///< The 1 key
    m_keyCodes["Num2"] = sf::Keyboard::Key::Num2;     ///< The 2 key
    m_keyCodes["Num3"] = sf::Keyboard::Key::Num3;     ///< The 3 key
    m_keyCodes["Num4"] = sf::Keyboard::Key::Num4;     ///< The 4 key
    m_keyCodes["Num5"] = sf::Keyboard::Key::Num5;     ///< The 5 key
    m_keyCodes["Num6"] = sf::Keyboard::Key::Num6;     ///< The 6 key
    m_keyCodes["Num7"] = sf::Keyboard::Key::Num7;     ///< The 7 key
    m_keyCodes["Num8"] = sf::Keyboard::Key::Num8;     ///< The 8 key
    m_keyCodes["Num9"] = sf::Keyboard::Key::Num9;     ///< The 9 key

    m_keyCodes["Escape"]    = sf::Keyboard::Key::Escape;     ///< The Escape key
    m_keyCodes["LControl"]  = sf::Keyboard::Key::LControl;   ///< The left Control key
    m_keyCodes["LShift"]    = sf::Keyboard::Key::LShift;     ///< The left Shift key
    m_keyCodes["LAlt"]      = sf::Keyboard::Key::LAlt;       ///< The left Alt key
    m_keyCodes["LSystem"]   = sf::Keyboard::Key::LSystem;    ///< The left OS specific key : windows (Windows and Linux), apple (MacOS X), ...
    m_keyCodes["RControl"]  = sf::Keyboard::Key::RControl;   ///< The right Control key
    m_keyCodes["RShift"]    = sf::Keyboard::Key::RShift;     ///< The right Shift key
    m_keyCodes["RAlt"]      = sf::Keyboard::Key::RAlt;       ///< The right Alt key
    m_keyCodes["RSystem"]   = sf::Keyboard::Key::RSystem;    ///< The right OS specific key : windows (Windows and Linux), apple (MacOS X), ...

    m_keyCodes["Menu"]      = sf::Keyboard::Key::Menu;       ///< The Menu key
    m_keyCodes["LBracket"]  = sf::Keyboard::Key::LBracket;   ///< The [ key
    m_keyCodes["RBracket"]  = sf::Keyboard::Key::RBracket;   ///< The ] key
    m_keyCodes["Semicolon"] = sf::Keyboard::Key::Semicolon;  ///< The ; key
    m_keyCodes["Comma"]     = sf::Keyboard::Key::Comma;      ///< The , key
    m_keyCodes["Period"]    = sf::Keyboard::Key::Period;     ///< The . key
    m_keyCodes["Quote"]     = sf::Keyboard::Key::Apostrophe; ///< The ' key
    m_keyCodes["Slash"]     = sf::Keyboard::Key::Slash;      ///< The / key
    m_keyCodes["Backslash"] = sf::Keyboard::Key::Backslash;  ///< The \ key
    m_keyCodes["Tilde"]     = sf::Keyboard::Key::Grave;      ///< The ~ key
    m_keyCodes["Equal"]     = sf::Keyboard::Key::Equal;      ///< The = key
    m_keyCodes["Hyphen"]    = sf::Keyboard::Key::Hyphen;     ///< The - key
    m_keyCodes["Space"]     = sf::Keyboard::Key::Space;      ///< The Space key
    m_keyCodes["Enter"]     = sf::Keyboard::Key::Enter;      ///< The Enter key
    m_keyCodes["Backspace"] = sf::Keyboard::Key::Backspace;  ///< The Backspace key
    m_keyCodes["Tab"]       = sf::Keyboard::Key::Tab;        ///< The Tabulation key
    m_keyCodes["PageUp"]    = sf::Keyboard::Key::PageUp;     ///< The Page up key
    m_keyCodes["PageDown"]  = sf::Keyboard::Key::PageDown;   ///< The Page down key
    m_keyCodes["End"]       = sf::Keyboard::Key::End;        ///< The End key
    m_keyCodes["Home"]      = sf::Keyboard::Key::Home;       ///< The Home key
    m_keyCodes["Insert"]    = sf::Keyboard::Key::Insert;     ///< The Insert key
    m_keyCodes["Delete"]    = sf::Keyboard::Key::Delete;     ///< The Delete key

    m_keyCodes["Add"]       = sf::Keyboard::Key::Add;        ///< +
    m_keyCodes["Subtract"]  = sf::Keyboard::Key::Subtract;   ///< -
    m_keyCodes["Multiply"]  = sf::Keyboard::Key::Multiply;   ///< *
    m_keyCodes["Divide"]    = sf::Keyboard::Key::Divide;     ///< /
    m_keyCodes["Left"]      = sf::Keyboard::Key::Left;       ///< Left arrow
    m_keyCodes["Right"]     = sf::Keyboard::Key::Right;      ///< Right arrow
    m_keyCodes["Up"]        = sf::Keyboard::Key::Up;         ///< Up arrow
    m_keyCodes["Down"]      = sf::Keyboard::Key::Down;       ///< Down arrow
    m_keyCodes["Numpad0"]   = sf::Keyboard::Key::Numpad0;    ///< The numpad 0 key
    m_keyCodes["Numpad1"]   = sf::Keyboard::Key::Numpad1;    ///< The numpad 1 key
    m_keyCodes["Numpad2"]   = sf::Keyboard::Key::Numpad2;    ///< The numpad 2 key
    m_keyCodes["Numpad3"]   = sf::Keyboard::Key::Numpad3;    ///< The numpad 3 key
    m_keyCodes["Numpad4"]   = sf::Keyboard::Key::Numpad4;    ///< The numpad 4 key
    m_keyCodes["Numpad5"]   = sf::Keyboard::Key::Numpad5;    ///< The numpad 5 key
    m_keyCodes["Numpad6"]   = sf::Keyboard::Key::Numpad6;    ///< The numpad 6 key
    m_keyCodes["Numpad7"]   = sf::Keyboard::Key::Numpad7;    ///< The numpad 7 key
    m_keyCodes["Numpad8"]   = sf::Keyboard::Key::Numpad8;    ///< The numpad 8 key
    m_keyCodes["Numpad9"]   = sf::Keyboard::Key::Numpad9;    ///< The numpad 9 key

    m_keyCodes["F1"]    = sf::Keyboard::Key::F1;     ///< The F1 key
    m_keyCodes["F2"]    = sf::Keyboard::Key::F2;     ///< The F2 key
    m_keyCodes["F3"]    = sf::Keyboard::Key::F3;     ///< The F3 key
    m_keyCodes["F4"]    = sf::Keyboard::Key::F4;     ///< The F4 key
    m_keyCodes["F5"]    = sf::Keyboard::Key::F5;     ///< The F5 key
    m_keyCodes["F6"]    = sf::Keyboard::Key::F6;     ///< The F6 key
    m_keyCodes["F7"]    = sf::Keyboard::Key::F7;     ///< The F7 key
    m_keyCodes["F8"]    = sf::Keyboard::Key::F8;     ///< The F8 key
    m_keyCodes["F9"]    = sf::Keyboard::Key::F9;     ///< The F8 key
    m_keyCodes["F10"]   = sf::Keyboard::Key::F10;    ///< The F10 key
    m_keyCodes["F11"]   = sf::Keyboard::Key::F11;    ///< The F11 key
    m_keyCodes["F12"]   = sf::Keyboard::Key::F12;    ///< The F12 key
    m_keyCodes["F13"]   = sf::Keyboard::Key::F13;    ///< The F13 key
    m_keyCodes["F14"]   = sf::Keyboard::Key::F14;    ///< The F14 key
    m_keyCodes["F15"]   = sf::Keyboard::Key::F15;    ///< The F15 key

    m_keyCodes["Pause"] = sf::Keyboard::Key::Pause;  ///< The Pause key
}

bool ManagerInputs::ReadKeyCode(const std::string& _strValue, sf::Keyboard::Key& _eKey) const
{
    auto kvp = m_keyCodes.find(_strValue);
    if (kvp != m_keyCodes.end())
    {
        _eKey = kvp->second;
        return true;
    }

    return false;
}

bool ManagerInputs::IsInputAllowed() const
{
    return GetGameWindow()->IsInputAllowed();
}

ManagerInputs* GetInputs()
{
    return GetEngine()->GetManagerInputs();
}

}   // namespace gugu
