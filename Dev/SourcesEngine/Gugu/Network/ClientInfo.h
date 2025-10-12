#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

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
    ClientInfo(uint32 _oIPAddress, uint16 _uiPort);
    ~ClientInfo();
    
public:

    sf::TcpSocket* m_socket;
    uint32 m_ipAddress;
    uint16 m_port;

    bool m_isHost;
    int32 m_playerID;
    uint32 m_lastTurnReceived;
};

}   // namespace gugu
