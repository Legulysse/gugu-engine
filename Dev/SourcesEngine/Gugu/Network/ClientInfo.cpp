////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Network/ClientInfo.h"

#include <SFML/Network/TcpSocket.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ClientInfo::ClientInfo()
{
    m_socket = nullptr;
    m_port = 0;

    m_isHost = false;
    m_playerID = -1;
    m_lastTurnReceived = 0;
}

ClientInfo::ClientInfo(sf::IpAddress _oIPAddress, sf::Uint16 _uiPort)
{
    m_socket = nullptr;
    m_ipAddress = _oIPAddress;
    m_port = _uiPort;

    m_isHost = false;
    m_playerID = -1;
    m_lastTurnReceived = 0;
}

ClientInfo::~ClientInfo()
{
    if(m_socket)
    {
        m_socket->disconnect();
        delete m_socket;
    }
}

}   // namespace gugu
