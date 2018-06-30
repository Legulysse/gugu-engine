////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Network/NetworkPacket.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Utility/System.h"

#include <SFML/Network/Packet.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
NetPacket::NetPacket()
{
	m_type = ENetPacket::Undefined;
}

NetPacket::NetPacket(ENetPacket::Type _eType)
{
	m_type = _eType;
}

NetPacket::~NetPacket()
{
}

void NetPacket::FillSFPacket(sf::Packet& _oSFPacket)
{
    _oSFPacket << static_cast<sf::Uint32>(m_type);
}

void NetPacket::ReadSFPacket(sf::Packet& _oSFPacket)
{
}

NetPacketClientConnection::NetPacketClientConnection(sf::IpAddress _oIPAddress, sf::Uint16 _uiPort)
{
	m_type = ENetPacket::ClientInfos;

    m_ipAddress	= _oIPAddress;
    m_port		= _uiPort;
}

NetPacketClientConnection::NetPacketClientConnection(sf::Packet& _oSFPacket)
{
	m_type = ENetPacket::ClientInfos;

	ReadSFPacket(_oSFPacket);
}

NetPacketClientConnection::~NetPacketClientConnection()
{
}

void NetPacketClientConnection::FillSFPacket(sf::Packet& _oSFPacket)
{
    NetPacket::FillSFPacket(_oSFPacket);

    _oSFPacket << m_ipAddress.toInteger() << m_port;
}

void NetPacketClientConnection::ReadSFPacket(sf::Packet& _oSFPacket)
{
    NetPacket::ReadSFPacket(_oSFPacket);

	sf::Uint32 uiAddress;
	_oSFPacket >> uiAddress >> m_port;

	sf::IpAddress oIPAddress( uiAddress );
	m_ipAddress = oIPAddress;
}

NetPacketAddPlayer::NetPacketAddPlayer(sf::IpAddress _oIPAddress, sf::Uint16 _uiPort, sf::Int32 _iPlayerID)
{
	m_type = ENetPacket::AddPlayer;

    m_ipAddress	= _oIPAddress;
    m_port		= _uiPort;
	m_playerID		= _iPlayerID;
}

NetPacketAddPlayer::NetPacketAddPlayer(sf::Packet& _oSFPacket)
{
	m_type = ENetPacket::AddPlayer;

	ReadSFPacket(_oSFPacket);
}

NetPacketAddPlayer::~NetPacketAddPlayer()
{
}

void NetPacketAddPlayer::FillSFPacket(sf::Packet& _oSFPacket)
{
    NetPacket::FillSFPacket(_oSFPacket);

    _oSFPacket << m_ipAddress.toInteger() << m_port << m_playerID;
}

void NetPacketAddPlayer::ReadSFPacket(sf::Packet& _oSFPacket)
{
    NetPacket::ReadSFPacket(_oSFPacket);

	sf::Uint32 uiAddress;
	_oSFPacket >> uiAddress >> m_port >> m_playerID;

	sf::IpAddress oIPAddress( uiAddress );
	m_ipAddress = oIPAddress;
}

NetPacketTurnReady::NetPacketTurnReady(sf::Uint32 _uiNetTurn)
{
	m_type = ENetPacket::TurnReady;

	m_turn = _uiNetTurn;
}

NetPacketTurnReady::NetPacketTurnReady(sf::Packet& _oSFPacket)
{
	m_type = ENetPacket::TurnReady;

	ReadSFPacket(_oSFPacket);
}

NetPacketTurnReady::~NetPacketTurnReady()
{
}

void NetPacketTurnReady::FillSFPacket(sf::Packet& _oSFPacket)
{
	NetPacket::FillSFPacket(_oSFPacket);

	_oSFPacket << m_turn;
}

void NetPacketTurnReady::ReadSFPacket(sf::Packet& _oSFPacket)
{
	NetPacket::ReadSFPacket(_oSFPacket);

	_oSFPacket >> m_turn;
}

NetPacketGame::NetPacketGame()
{
	m_type = ENetPacket::Game;
	m_turn = 0;
}

NetPacketGame::NetPacketGame(sf::Packet& _oSFPacket)
{
	m_type = ENetPacket::Game;
	m_turn = 0;

    ReadSFPacket(_oSFPacket);
}

NetPacketGame::~NetPacketGame()
{
}

void NetPacketGame::FillSFPacket(sf::Packet& _oSFPacket)
{
    NetPacket::FillSFPacket(_oSFPacket);

	_oSFPacket << m_turn;
}

void NetPacketGame::ReadSFPacket(sf::Packet& _oSFPacket)
{
    NetPacket::ReadSFPacket(_oSFPacket);

	_oSFPacket >> m_turn;
}

}   // namespace gugu
