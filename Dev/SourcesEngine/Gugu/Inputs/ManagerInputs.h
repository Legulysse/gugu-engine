#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

#include <SFML/Window/Event.hpp>

#include <vector>
#include <map>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineConfig;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

enum class EPadButton : int8
{
    X360_A = 0,
    X360_B = 1,
    X360_X = 2,
    X360_Y = 3,
    X360_LeftShoulder = 4,
    X360_RightShoulder = 5,
    X360_Back = 6,
    X360_Start = 7,
    X360_L3 = 8,
    X360_R3 = 9,
};

enum class EPadAxis : int8
{
    X360_LeftStick_X = sf::Joystick::Axis::X,
    X360_LeftStick_Y = sf::Joystick::Axis::Y,
    X360_Triggers = sf::Joystick::Axis::Z,  // positive = left, negative = right
    // sf::Joystick::Axis::R,   // unused
    X360_RightStick_X = sf::Joystick::Axis::U,
    X360_RightStick_Y = sf::Joystick::Axis::V,
    X360_DPad_X = sf::Joystick::Axis::PovX,
    X360_DPad_Y = sf::Joystick::Axis::PovY,
};

struct InputKeyBindings
{
    std::vector<sf::Event> bindings;
};

class ManagerInputs
{
public:

    ManagerInputs();
    ~ManagerInputs();

    void Init(const EngineConfig& config);
    void Release();

    void RegisterInput  (const std::string& _strInputName, const sf::Event& _oSFEvent);                         //Will add a new key to the specified input.
    bool ModifyInput    (const std::string& _strInputName, const sf::Event& _oSFEvent, uint32 _uiIndex = 0);    //Will modify the existing key associated to the given input. Return false if input, with given index, doesn't exists.

    void LoadInputFile  (const std::string& _strPath);

    bool IsInput            (const std::string& _strInputName, const sf::Event& _oSFEvent) const;
    bool IsInputPressed     (const std::string& _strInputName, const sf::Event& _oSFEvent) const;
    bool IsInputReleased    (const std::string& _strInputName, const sf::Event& _oSFEvent) const;
    bool IsInputDown        (const std::string& _strInputName) const;
    bool IsShiftDown        () const;

    bool IsKeyDown          (sf::Keyboard::Key _eKey) const;

    static sf::Event BuildKeyboardEvent(sf::Keyboard::Key key);
    static sf::Event BuildJoystickEvent(EPadButton button, int joystickId = 0);
    static sf::Event BuildJoystickEvent(int button, int joystickId = 0);

private:

    void FillListKeyCodes();
    bool ReadKeyCode(const std::string& _strValue, sf::Keyboard::Key& _eKey) const;

private:

    std::map<std::string, InputKeyBindings> m_inputBindings;
    std::map<std::string, sf::Keyboard::Key> m_keyCodes;
};

ManagerInputs* GetInputs();

}   // namespace gugu
