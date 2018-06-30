#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Network/EnumsNetwork.h"

#include <SFML/Network/IpAddress.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
    class Packet;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class NetPacket
{
public:

	NetPacket();
	NetPacket(ENetPacket::Type _eType);
	virtual ~NetPacket();

	virtual void FillSFPacket(sf::Packet& _oSFPacket);
	virtual void ReadSFPacket(sf::Packet& _oSFPacket);

public:

	ENetPacket::Type m_type;
};
    
class NetPacketClientConnection : public NetPacket
{
public:

	NetPacketClientConnection(sf::IpAddress _oIPAddress, sf::Uint16 _uiPort);
	NetPacketClientConnection(sf::Packet& _oSFPacket);
	virtual ~NetPacketClientConnection();

    virtual void FillSFPacket(sf::Packet& _oSFPacket) override;
    virtual void ReadSFPacket(sf::Packet& _oSFPacket) override;
    
public:

	sf::IpAddress m_ipAddress;
	sf::Uint16 m_port;
};

class NetPacketAddPlayer : public NetPacket
{
public:

	NetPacketAddPlayer(sf::IpAddress _oIPAddress, sf::Uint16 _uiPort, sf::Int32 _iPlayerID);
	NetPacketAddPlayer(sf::Packet& _oSFPacket);
	virtual ~NetPacketAddPlayer();

    virtual void FillSFPacket(sf::Packet& _oSFPacket) override;
    virtual void ReadSFPacket(sf::Packet& _oSFPacket) override;

public:

	sf::IpAddress m_ipAddress;
	sf::Uint16 m_port;
	sf::Int32 m_playerID;
};

class NetPacketTurnReady : public NetPacket
{
public:

	NetPacketTurnReady(sf::Uint32 _uiNetTurn);
	NetPacketTurnReady(sf::Packet& _oSFPacket);
	virtual ~NetPacketTurnReady();

    virtual void FillSFPacket(sf::Packet& _oSFPacket) override;
    virtual void ReadSFPacket(sf::Packet& _oSFPacket) override;
    
public:

	sf::Uint32 m_turn;
};

class NetPacketGame : public NetPacket
{
public:

	NetPacketGame();
	NetPacketGame(sf::Packet& _oSFPacket);
	virtual ~NetPacketGame();

    virtual void FillSFPacket(sf::Packet& _oSFPacket) override;
    virtual void ReadSFPacket(sf::Packet& _oSFPacket) override;
    
public:

	sf::Uint32	m_turn;
};

}   // namespace gugu
