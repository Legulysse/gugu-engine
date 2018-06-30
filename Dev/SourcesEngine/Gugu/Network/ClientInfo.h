#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/Network/IpAddress.hpp>

#include <list>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
    class TcpSocket;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ClientInfo
{
public:

	ClientInfo();
	ClientInfo(sf::IpAddress _oIPAddress, sf::Uint16 _uiPort);
	~ClientInfo();
    
public:

	sf::TcpSocket*	m_socket;
	sf::IpAddress	m_ipAddress;
	sf::Uint16		m_port;

    bool            m_isHost;
	sf::Int32		m_playerID;
	sf::Uint32		m_lastTurnReceived;
};

}   // namespace gugu
