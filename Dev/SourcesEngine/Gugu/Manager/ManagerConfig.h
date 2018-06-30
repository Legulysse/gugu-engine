#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/Types.h"

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

struct InputKeyBindings
{
	std::vector<sf::Event> bindings;
};

class ManagerConfig
{
public:

    ManagerConfig();
    ~ManagerConfig();

    void Init(const EngineConfig& config);
    void Release();

	void RegisterInput	(const std::string& _strInputName, const sf::Event& _oSFEvent);							//Will add a new key to the specified input.
	bool ModifyInput	(const std::string& _strInputName, const sf::Event& _oSFEvent, uint32 _uiIndex = 0);	//Will modify the existing key associated to the given input. Return false if input, with given index, doesn't exists.

	void LoadInputFile	(const std::string& _strPath);

	bool IsInput			(const std::string& _strInputName, const sf::Event& _oSFEvent) const;
	bool IsInputPressed		(const std::string& _strInputName, const sf::Event& _oSFEvent) const;
	bool IsInputReleased	(const std::string& _strInputName, const sf::Event& _oSFEvent) const;
	bool IsInputDown		(const std::string& _strInputName) const;
	bool IsShiftDown		() const;

    bool IsKeyDown		    (sf::Keyboard::Key _eKey) const;

	sf::Event	BuildEventKey	(sf::Keyboard::Key _eKey);

private:

	void FillListKeyCodes();
    bool ReadKeyCode(const std::string& _strValue, sf::Keyboard::Key& _eKey) const;

private:

    std::map<std::string, InputKeyBindings> m_inputBindings;
    std::map<std::string, sf::Keyboard::Key> m_keyCodes;
};

ManagerConfig* GetConfig();

}   // namespace gugu
