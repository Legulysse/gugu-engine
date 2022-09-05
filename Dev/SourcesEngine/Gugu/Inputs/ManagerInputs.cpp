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
    pugi::xml_parse_result result = oDoc.load_file(GetResources()->GetResourcePathName(_strPath).c_str());
    if (!result)
        return;

    pugi::xml_node oNodeBindings = oDoc.child("Bindings");
    if (!oNodeBindings)
        return;
    
    std::string strInputName, strKey;
    sf::Keyboard::Key oKeyCode = sf::Keyboard::Unknown;

    for (pugi::xml_node oNodeInput = oNodeBindings.child("Input"); oNodeInput; oNodeInput = oNodeInput.next_sibling("Input"))
    {
        pugi::xml_attribute oAttributeName = oNodeInput.attribute("name");
        if (!oAttributeName)
            continue;

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
    return IsInputAllowed() && ((_oSFEvent.type == sf::Event::KeyPressed || _oSFEvent.type == sf::Event::JoystickButtonPressed) && IsInputEvent(_strInputName, _oSFEvent));
}

bool ManagerInputs::IsInputEventReleased(const std::string& _strInputName, const sf::Event& _oSFEvent) const
{
    return IsInputAllowed() && ((_oSFEvent.type == sf::Event::KeyReleased || _oSFEvent.type == sf::Event::JoystickButtonReleased) && IsInputEvent(_strInputName, _oSFEvent));
}

bool ManagerInputs::IsControlDown() const
{
    return IsInputAllowed() && (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl));
}

bool ManagerInputs::IsShiftDown() const
{
    return IsInputAllowed() && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
}

bool ManagerInputs::IsAltDown() const
{
    return IsInputAllowed() && (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt));
}

bool ManagerInputs::IsKeyDown(sf::Keyboard::Key _eKey) const
{
    return IsInputAllowed() && sf::Keyboard::isKeyPressed(_eKey);
}

bool ManagerInputs::IsButtonDown(sf::Mouse::Button button) const
{
    return IsInputAllowed() && sf::Mouse::isButtonPressed(button);
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
    m_keyCodes["A"] = sf::Keyboard::A;
    m_keyCodes["B"] = sf::Keyboard::B;
    m_keyCodes["C"] = sf::Keyboard::C;
    m_keyCodes["D"] = sf::Keyboard::D;
    m_keyCodes["E"] = sf::Keyboard::E;
    m_keyCodes["F"] = sf::Keyboard::F;
    m_keyCodes["G"] = sf::Keyboard::G;
    m_keyCodes["H"] = sf::Keyboard::H;
    m_keyCodes["I"] = sf::Keyboard::I;
    m_keyCodes["J"] = sf::Keyboard::J;
    m_keyCodes["K"] = sf::Keyboard::K;
    m_keyCodes["L"] = sf::Keyboard::L;
    m_keyCodes["M"] = sf::Keyboard::M;
    m_keyCodes["N"] = sf::Keyboard::N;
    m_keyCodes["O"] = sf::Keyboard::O;
    m_keyCodes["P"] = sf::Keyboard::P;
    m_keyCodes["Q"] = sf::Keyboard::Q;
    m_keyCodes["R"] = sf::Keyboard::R;
    m_keyCodes["S"] = sf::Keyboard::S;
    m_keyCodes["T"] = sf::Keyboard::T;
    m_keyCodes["U"] = sf::Keyboard::U;
    m_keyCodes["V"] = sf::Keyboard::V;
    m_keyCodes["W"] = sf::Keyboard::W;
    m_keyCodes["X"] = sf::Keyboard::X;
    m_keyCodes["Y"] = sf::Keyboard::Y;
    m_keyCodes["Z"] = sf::Keyboard::Z;

    m_keyCodes["Num0"] = sf::Keyboard::Num0;     ///< The 0 key
    m_keyCodes["Num1"] = sf::Keyboard::Num1;     ///< The 1 key
    m_keyCodes["Num2"] = sf::Keyboard::Num2;     ///< The 2 key
    m_keyCodes["Num3"] = sf::Keyboard::Num3;     ///< The 3 key
    m_keyCodes["Num4"] = sf::Keyboard::Num4;     ///< The 4 key
    m_keyCodes["Num5"] = sf::Keyboard::Num5;     ///< The 5 key
    m_keyCodes["Num6"] = sf::Keyboard::Num6;     ///< The 6 key
    m_keyCodes["Num7"] = sf::Keyboard::Num7;     ///< The 7 key
    m_keyCodes["Num8"] = sf::Keyboard::Num8;     ///< The 8 key
    m_keyCodes["Num9"] = sf::Keyboard::Num9;     ///< The 9 key

    m_keyCodes["Escape"]    = sf::Keyboard::Escape;     ///< The Escape key
    m_keyCodes["LControl"]  = sf::Keyboard::LControl;   ///< The left Control key
    m_keyCodes["LShift"]    = sf::Keyboard::LShift;     ///< The left Shift key
    m_keyCodes["LAlt"]      = sf::Keyboard::LAlt;       ///< The left Alt key
    m_keyCodes["LSystem"]   = sf::Keyboard::LSystem;    ///< The left OS specific key : windows (Windows and Linux), apple (MacOS X), ...
    m_keyCodes["RControl"]  = sf::Keyboard::RControl;   ///< The right Control key
    m_keyCodes["RShift"]    = sf::Keyboard::RShift;     ///< The right Shift key
    m_keyCodes["RAlt"]      = sf::Keyboard::RAlt;       ///< The right Alt key
    m_keyCodes["RSystem"]   = sf::Keyboard::RSystem;    ///< The right OS specific key : windows (Windows and Linux), apple (MacOS X), ...

    m_keyCodes["Menu"]      = sf::Keyboard::Menu;       ///< The Menu key
    m_keyCodes["LBracket"]  = sf::Keyboard::LBracket;   ///< The [ key
    m_keyCodes["RBracket"]  = sf::Keyboard::RBracket;   ///< The ] key
    m_keyCodes["Semicolon"] = sf::Keyboard::Semicolon;  ///< The ; key
    m_keyCodes["Comma"]     = sf::Keyboard::Comma;      ///< The , key
    m_keyCodes["Period"]    = sf::Keyboard::Period;     ///< The . key
    m_keyCodes["Quote"]     = sf::Keyboard::Quote;      ///< The ' key
    m_keyCodes["Slash"]     = sf::Keyboard::Slash;      ///< The / key
    m_keyCodes["Backslash"] = sf::Keyboard::Backslash;  ///< The \ key
    m_keyCodes["Tilde"]     = sf::Keyboard::Tilde;      ///< The ~ key
    m_keyCodes["Equal"]     = sf::Keyboard::Equal;      ///< The = key
    m_keyCodes["Hyphen"]    = sf::Keyboard::Hyphen;     ///< The - key
    m_keyCodes["Space"]     = sf::Keyboard::Space;      ///< The Space key
    m_keyCodes["Enter"]     = sf::Keyboard::Enter;      ///< The Enter key
    m_keyCodes["Backspace"] = sf::Keyboard::Backspace;  ///< The Backspace key
    m_keyCodes["Tab"]       = sf::Keyboard::Tab;        ///< The Tabulation key
    m_keyCodes["PageUp"]    = sf::Keyboard::PageUp;     ///< The Page up key
    m_keyCodes["PageDown"]  = sf::Keyboard::PageDown;   ///< The Page down key
    m_keyCodes["End"]       = sf::Keyboard::End;        ///< The End key
    m_keyCodes["Home"]      = sf::Keyboard::Home;       ///< The Home key
    m_keyCodes["Insert"]    = sf::Keyboard::Insert;     ///< The Insert key
    m_keyCodes["Delete"]    = sf::Keyboard::Delete;     ///< The Delete key

    m_keyCodes["Add"]       = sf::Keyboard::Add;        ///< +
    m_keyCodes["Subtract"]  = sf::Keyboard::Subtract;   ///< -
    m_keyCodes["Multiply"]  = sf::Keyboard::Multiply;   ///< *
    m_keyCodes["Divide"]    = sf::Keyboard::Divide;     ///< /
    m_keyCodes["Left"]      = sf::Keyboard::Left;       ///< Left arrow
    m_keyCodes["Right"]     = sf::Keyboard::Right;      ///< Right arrow
    m_keyCodes["Up"]        = sf::Keyboard::Up;         ///< Up arrow
    m_keyCodes["Down"]      = sf::Keyboard::Down;       ///< Down arrow
    m_keyCodes["Numpad0"]   = sf::Keyboard::Numpad0;    ///< The numpad 0 key
    m_keyCodes["Numpad1"]   = sf::Keyboard::Numpad1;    ///< The numpad 1 key
    m_keyCodes["Numpad2"]   = sf::Keyboard::Numpad2;    ///< The numpad 2 key
    m_keyCodes["Numpad3"]   = sf::Keyboard::Numpad3;    ///< The numpad 3 key
    m_keyCodes["Numpad4"]   = sf::Keyboard::Numpad4;    ///< The numpad 4 key
    m_keyCodes["Numpad5"]   = sf::Keyboard::Numpad5;    ///< The numpad 5 key
    m_keyCodes["Numpad6"]   = sf::Keyboard::Numpad6;    ///< The numpad 6 key
    m_keyCodes["Numpad7"]   = sf::Keyboard::Numpad7;    ///< The numpad 7 key
    m_keyCodes["Numpad8"]   = sf::Keyboard::Numpad8;    ///< The numpad 8 key
    m_keyCodes["Numpad9"]   = sf::Keyboard::Numpad9;    ///< The numpad 9 key

    m_keyCodes["F1"]    = sf::Keyboard::F1;     ///< The F1 key
    m_keyCodes["F2"]    = sf::Keyboard::F2;     ///< The F2 key
    m_keyCodes["F3"]    = sf::Keyboard::F3;     ///< The F3 key
    m_keyCodes["F4"]    = sf::Keyboard::F4;     ///< The F4 key
    m_keyCodes["F5"]    = sf::Keyboard::F5;     ///< The F5 key
    m_keyCodes["F6"]    = sf::Keyboard::F6;     ///< The F6 key
    m_keyCodes["F7"]    = sf::Keyboard::F7;     ///< The F7 key
    m_keyCodes["F8"]    = sf::Keyboard::F8;     ///< The F8 key
    m_keyCodes["F9"]    = sf::Keyboard::F9;     ///< The F8 key
    m_keyCodes["F10"]   = sf::Keyboard::F10;    ///< The F10 key
    m_keyCodes["F11"]   = sf::Keyboard::F11;    ///< The F11 key
    m_keyCodes["F12"]   = sf::Keyboard::F12;    ///< The F12 key
    m_keyCodes["F13"]   = sf::Keyboard::F13;    ///< The F13 key
    m_keyCodes["F14"]   = sf::Keyboard::F14;    ///< The F14 key
    m_keyCodes["F15"]   = sf::Keyboard::F15;    ///< The F15 key

    m_keyCodes["Pause"] = sf::Keyboard::Pause;  ///< The Pause key
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
